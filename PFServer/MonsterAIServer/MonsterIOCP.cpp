#include "MonsterIOCP.h"
#include "PacketProcess.h"

#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstdlib> //rand(), srand()
#include <ctime> //time()


PacketProcess		MonsterIOCP::packetProc;


unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	MonsterIOCP* pOverlappedEvent = (MonsterIOCP*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}


MonsterIOCP::MonsterIOCP()
{
	
}

MonsterIOCP::~MonsterIOCP()
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

}

bool MonsterIOCP::Initialize()
{
	IOCPBase::Initialize();
	int nResult;
	// 서버 정보 설정
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	// 소켓 설정
	// boost bind 와 구별짓기 위해 ::bind 사용
	nResult = ::bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));

	if (nResult == SOCKET_ERROR)
	{
		printf_s("ERROR::bind 실패\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// 수신 대기열 생성
	nResult = listen(ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		printf_s("ERROR::listen 실패\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void MonsterIOCP::StartServer()
{
	IOCPBase::StartServer();
}

bool MonsterIOCP::CreateWorkerThread()
{
	unsigned int threadId;
	// 시스템 정보 가져옴
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf_s("INFO::CPU 갯수 : %d\n", sysInfo.dwNumberOfProcessors);
	// 적절한 작업 스레드의 갯수는 (CPU * 2) + 1
	nThreadCnt = sysInfo.dwNumberOfProcessors;

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
			printf_s("ERROR::Monster Worker Thread 생성 실패\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	printf_s("INFO::Monster Worker Thread 시작...\n");
	return true;
}

void MonsterIOCP::WorkerThread()
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

	//몬스터 관련 스레드
	//초기 몬스터 세팅
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
			//패킷 종류
			int nPacketCode;

			//전송받은 스트링 스트림
			stringstream ReceiveStream;

			// 패킷 처리 구문
			// 클라이언트 정보 역직렬화
			ReceiveStream << pSocketInfo->dataBuf.buf;
			ReceiveStream >> nPacketCode;

			switch (nPacketCode)
			{
			case EPacketType::ENROLL_PLAYER:
				packetProc.EnrollCharacter(ReceiveStream, pSocketInfo);
				break;
			
			case EPacketType::SEND_PLAYER:
				packetProc.ReceiveCharacter(ReceiveStream, pSocketInfo);
				break;
			default:
				printf_s("ERROR::정의 되지 않은 패킷 : %d\n", nPacketCode);
				break;
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

void MonsterIOCP::Send(stSOCKETINFO* pSocket)
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

//void MonsterIOCP::MonsterMainThread()
//{
//	//몬스터 리젠 여부
//	bool bIsMonsterRespawn = true;
//	//기본 딜레이
//
//	//몬스터 전부가 죽었는지
//	int nMonsterAliveCount = monInfo.monsters.size();
//
//	printf_s("info::잔여 몬스터 수 : %d\n", nMonsterAliveCount);
//
//	//몬스터 근처에 유저가 있는지 판별
//	for (auto& kvp : monInfo.monsters)
//	{
//		auto& monMap = kvp.second;
//		MonsterVO* monster = &monInfo.monsters[monMap.Id];
//
//		//접속한 플레이어가 없거나 몬스터가 죽었을 경우 실행하지 않는다.
//		if (cInfo.players.size() <= 0 || !monster->IsAlive())
//			continue;
//
//		monster->SetPlayerInTrackingInfo(cInfo.players);
//	}
//
//	for (auto& monMap : monInfo.monsters)
//	{
//		auto& mon = monMap.second;
//		MonsterVO* monster = &monInfo.monsters[mon.Id];
//
//		if (monster->isPlayerInTraceRange)
//		{
//			if (monster->isPlayerInHitRange)
//			{
//				if (monster->MonsterCond == ECondition::IS_ATTACK)
//				{
//					monster->MonsterCond = ECondition::IS_IDLE;
//				}
//				else
//				{
//					monster->MonsterCond = ECondition::IS_ATTACK;
//				}
//			}
//
//			if (monster->MonsterCond == ECondition::IS_IDLE)
//			{
//				monster->MonsterCond = ECondition::IS_MOVE;
//				monster->ChangeLocToDestLoc();
//			}
//		}
//		else if (!monster->isPlayerInTraceRange &&
//			!monster->IsOriginPosition())
//		{
//			monster->MoveOri();
//			monster->MonsterCond = ECondition::IS_MOVE;
//		}
//		else
//		{
//			monster->MonsterCond = ECondition::IS_IDLE;
//		}
//	}
//}

//void MonsterIOCP::Broadcast(stringstream& SendStream)
//{
//	stSOCKETINFO* client = new stSOCKETINFO;
//	for (const auto& kvp : SessionSocket)
//	{
//		client->socket = kvp.second;
//		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
//		client->dataBuf.buf = client->messageBuffer;
//		client->dataBuf.len = SendStream.str().length();
//
//		Send(client);
//	}
//}


//void MonsterIOCP::Logout(stringstream& RecvStream, stSOCKETINFO* pSocket)
//{
//	int SessionId;
//	RecvStream >> SessionId;
//	printf_s("INFO::(%d)로그아웃 요청 수신\n", SessionId);
//	EnterCriticalSection(&csPlayers);
//
//	//잔여 플레이어 목록 배열에서 해당 SessionID키값에 해당하는 데이터를 지운다
//	cInfo.players.erase(SessionId);
//
//	LeaveCriticalSection(&csPlayers);
//	SessionSocket.erase(SessionId);
//	printf_s("INFO::클라이언트 수 : %d\n", SessionSocket.size());
//}