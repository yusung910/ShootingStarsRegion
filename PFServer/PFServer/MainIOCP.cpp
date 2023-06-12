#include "MainIOCP.h"

#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstdlib> //rand(), srand()
#include <ctime> //time()
// static 변수 설정
map<int, SOCKET>	MainIOCP::SessionSocket;
DataAccess			MainIOCP::Dao;
CRITICAL_SECTION	MainIOCP::csPlayers;
PlayerVO			MainIOCP::vo;
CharacterInfo		MainIOCP::cInfo;
MonsterSet			MainIOCP::monInfo;


unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	MainIOCP* pOverlappedEvent = (MainIOCP*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

unsigned int WINAPI CallMonsterThread(LPVOID p)
{
	MainIOCP* pOverlappedEvent = (MainIOCP*)p;
	pOverlappedEvent->MonsterManagementThread();
	return 0;
}


MainIOCP::MainIOCP()
{
	InitializeCriticalSection(&csPlayers);

	// 패킷 함수 포인터 배열 패킷별 함수 지정
	fnProcess[EPacketType::LOGIN].funcProcessPacket = PacketProcesses::Login;
	fnProcess[EPacketType::SEARCH_CHARACTER].funcProcessPacket = PacketProcesses::SearchCharacters;
	fnProcess[EPacketType::LOGOUT].funcProcessPacket = PacketProcesses::Logout;
	fnProcess[EPacketType::CREATE_CHARACTER].funcProcessPacket = PacketProcesses::CreateCharacter;
	fnProcess[EPacketType::DELETE_CHARACTER].funcProcessPacket = PacketProcesses::DeleteCharacter;
	fnProcess[EPacketType::ENROLL_PLAYER].funcProcessPacket = PacketProcesses::EnrollCharacter;
	fnProcess[EPacketType::SEND_PLAYER].funcProcessPacket = PacketProcesses::SyncCharacters;
	fnProcess[EPacketType::HIT_PLAYER].funcProcessPacket = PacketProcesses::HitCharacter;
	fnProcess[EPacketType::HIT_MONSTER].funcProcessPacket = PacketProcesses::HitMonster;
	fnProcess[EPacketType::CHAT].funcProcessPacket = PacketProcesses::BroadcastChat;

}

MainIOCP::~MainIOCP()
{
	// winsock 의 사용을 끝낸다
	WSACleanup();
	// 다 사용한 객체를 삭제
	if (SocketInfo)
	{
		delete[] SocketInfo;
		SocketInfo = NULL;
	}

	if (hWorkerHandle)
	{
		delete[] hWorkerHandle;
		hWorkerHandle = NULL;
	}

	//임계영역 해제
	DeleteCriticalSection(&csPlayers);
}

void MainIOCP::StartServer()
{
	//23.06.06
	//몬스터 쓰레드 진행 시 서버에서 가지고 있는 몬스터들의 정보(monInfo)를
	//클라이언트에서 실시간으로 위치 정보를 패킷으로 전달 받으려고 했으나
	//서버에서 진행중인 쓰레드가 사용하고 있는 중에
	//클라이언트에서 전송 받은 정보를 같은 변수에다 덮어쓰려다가 에러 발생
	CreateMonsterManagementThread();
	IOCPBase::StartServer();
}


bool MainIOCP::CreateWorkerThread()
{
	unsigned int threadId;
	// 시스템 정보 가져옴
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf_s("INFO::CPU 갯수 : %d\n", sysInfo.dwNumberOfProcessors);
	// 적절한 작업 스레드의 갯수는 (CPU * 2) + 1
	nThreadCnt = sysInfo.dwNumberOfProcessors * 2;

	// thread handler 선언
	hWorkerHandle = new HANDLE[nThreadCnt];
	// thread 생성
	for (int i = 0; i < nThreadCnt; i++)
	{
		hWorkerHandle[i] = (HANDLE*)_beginthreadex(
			NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId
		);
		if (hWorkerHandle[i] == NULL)
		{
			printf_s("ERROR::Worker Thread 생성 실패\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	printf_s("INFO::Worker Thread 시작...\n");
	return true;
}

void MainIOCP::WorkerThread()
{
	// 함수 호출 성공 여부
	BOOL	bResult;
	int		nResult;
	// Overlapped I/O 작업에서 전송된 데이터 크기
	DWORD	recvBytes;
	DWORD	sendBytes;
	// Completion Key를 받을 포인터 변수
	stSOCKETINFO* pCompletionKey;
	// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터	
	stSOCKETINFO* pSocketInfo;
	DWORD	dwFlags = 0;

	while (bWorkerThread)
	{
		/**
		 * 이 함수로 인해 쓰레드들은 WaitingThread Queue 에 대기상태로 들어가게 됨
		 * 완료된 Overlapped I/O 작업이 발생하면 IOCP Queue 에서 완료된 작업을 가져와
		 * 뒷처리를 함
		 */
		bResult = GetQueuedCompletionStatus(hIOCP,
			&recvBytes,						// 실제로 전송된 바이트
			(PULONG_PTR)&pCompletionKey,	// completion key
			(LPOVERLAPPED*)&pSocketInfo,	// overlapped I/O 객체
			INFINITE						// 대기할 시간
		);

		if (!bResult && recvBytes == 0)
		{
			printf_s("INFO::socket(%d) 접속 끊김\n", pSocketInfo->socket);
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		pSocketInfo->dataBuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		try
		{
			// 패킷 종류
			int PacketType;
			// 클라이언트 정보 역직렬화
			stringstream RecvStream;

			RecvStream << pSocketInfo->dataBuf.buf;
			RecvStream >> PacketType;

			// 패킷 처리
			if (PacketType > -1 && PacketType < EPacketType::PACKET_TOT_AMOUNT)
			{

				if (nullptr != fnProcess[PacketType].funcProcessPacket)
				{
					fnProcess[PacketType].funcProcessPacket(RecvStream, pSocketInfo);
				}
				else
				{
					printf_s("ERROR::정의 되지 않은 패킷 : %d\n", PacketType);
				}
			}
		}
		catch (const std::exception& e)
		{
			printf_s("ERROR::알 수 없는 예외 발생 : %s\n", e.what());
		}

		// 클라이언트 대기
		Recv(pSocketInfo);
	}
}

void MainIOCP::WriteCharactersInfoToSocket(stSOCKETINFO* pSocket)
{
	//왜인지 모르겠지만 잔여 데이터 때문에 클리어해줘야함
	stringstream SendStream;
	// 직렬화	
	SendStream << EPacketType::RECV_PLAYER << endl;
	SendStream << cInfo << endl;

	// !!! 중요 !!! data.buf 에다 직접 데이터를 쓰면 쓰레기값이 전달될 수 있음
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();

}

void MainIOCP::Send(stSOCKETINFO* pSocket)
{
	int nResult;
	DWORD	sendBytes;
	DWORD	dwFlags = 0;

	nResult = WSASend(
		pSocket->socket,
		&(pSocket->dataBuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);


	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("ERROR::WSASend 실패 : %d \n", WSAGetLastError());
	}
}


void MainIOCP::InitializeMonsterSet()
{
	srand((unsigned int)time(NULL));

	// 몬스터 초기화	
	MonsterVO mFields;

	mFields.X = -2654;
	mFields.Y = 3629;
	mFields.Z = -461;

	mFields.ORI_X = -2654;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -461;

	mFields.MAX_HP = 100.0f;
	mFields.CUR_HP = 100.0f;
	mFields.Id = rand();
	mFields.Damage = 25.0f;

	monInfo.monsters[mFields.Id] = mFields;

	mFields.X = -1374;
	mFields.Y = 3629;
	mFields.Z = -520;

	mFields.ORI_X = -1374;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -520;

	mFields.Id = rand();
	monInfo.monsters[mFields.Id] = mFields;

	mFields.X = -724;
	mFields.Y = 3629;
	mFields.Z = -520;

	mFields.ORI_X = -724;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -520;
	mFields.Id = rand();
	monInfo.monsters[mFields.Id] = mFields;

	mFields.X = -830;
	mFields.Y = 1710;
	mFields.Z = -494;

	mFields.ORI_X = -1160;
	mFields.ORI_Y = 1709;
	mFields.ORI_Z = -410;
	mFields.Id = rand();
	monInfo.monsters[mFields.Id] = mFields;
}

void MainIOCP::MonsterManagementThread()
{
	//초기 몬스터 세팅
	InitializeMonsterSet();

	//몬스터 리젠 여부
	bool bIsMonsterRespawn = true;
	//기본 딜레이
	DWORD fThreadDelay = 500.0;
	//몬스터 전부가 죽었는지
	int nMonsterAliveCount = MainIOCP::monInfo.monsters.size();

	// 로직 시작
	while (true)
	{
		if (nMonsterAliveCount == 0)
		{
			Sleep(5 * 1000);
			InitializeMonsterSet();
		}


		nMonsterAliveCount = MainIOCP::monInfo.monsters.size();

		//몬스터 근처에 유저가 있는지 판별
		for (auto& kvp : monInfo.monsters)
		{
			auto& monMap = kvp.second;
			MonsterVO* monster = &MainIOCP::monInfo.monsters[monMap.Id];

			//접속한 플레이어가 없거나 몬스터가 죽었을 경우 실행하지 않는다.
			if (cInfo.players.size() <= 0 || !monster->IsAlive())
				continue;

			monster->SetPlayerInTrackingInfo(cInfo.players);
		}

		for (auto& monMap : monInfo.monsters)
		{
			auto& mon = monMap.second;
			MonsterVO* monster = &MainIOCP::monInfo.monsters[mon.Id];
			
			if (monster->isPlayerInTraceRange)
			{
				if (monster->isPlayerInHitRange)
				{
					if (monster->MonsterCond == ECondition::IS_ATTACK)
					{
						monster->MonsterCond = ECondition::IS_IDLE;
					}
					else
					{
						monster->MonsterCond = ECondition::IS_ATTACK;
					}
				}

				if(monster->MonsterCond == ECondition::IS_IDLE)
				{
					monster->MonsterCond = ECondition::IS_MOVE;
					monster->ChangeLocToDestLoc();
				}
			}
			else if(!monster->isPlayerInTraceRange &&
					!monster->IsOriginPosition())
			{
				monster->MoveOri();
				monster->MonsterCond = ECondition::IS_MOVE;
			}
			else
			{
				monster->MonsterCond = ECondition::IS_IDLE;
			}

		}

		PacketProcesses::SpawnAllMonsters();
		Sleep(fThreadDelay);
	}
}

void MainIOCP::CreateMonsterManagementThread()
{
	unsigned int threadId;

	MonsterHandle = (HANDLE*)_beginthreadex(
		NULL, 0, &CallMonsterThread, this, CREATE_SUSPENDED, &threadId
	);
	if (MonsterHandle == NULL)
	{
		printf_s("ERROR::Monster Thread 생성 실패\n");
		return;
	}
	ResumeThread(MonsterHandle);

	printf_s("ERROR::Monster Thread 시작...\n");
}
