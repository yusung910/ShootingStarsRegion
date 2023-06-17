#pragma once

#include "MonsterIOCP.h"
#include "PlayerVO.h"
#include "PacketProcess.h"

map<int, SOCKET>    PacketProcess::SessionSocket;	// 세션별 소켓 저장
CharacterInfo PacketProcess::playerInfo;			// 접속한 클라이언트의 정보를 저장	
CRITICAL_SECTION PacketProcess::criticalSec;		// 임계영역
MonsterSet	PacketProcess::monInfo;					// 몬스터정보
PacketProcess::PacketProcess()
{
	monsterIOCP = nullptr;
	printf_s("INFO::PacketController 생성\n");
	InitializeCriticalSection(&criticalSec);
	printf_s("INFO::Critical Section 설정\n");
}

PacketProcess::~PacketProcess()
{
	printf_s("INFO::PacketController 소멸\n");
	DeleteCriticalSection(&criticalSec);
	printf_s("INFO::Critical Section 해제\n");

}

void PacketProcess::EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	PlayerVO vo;
	RecvStream >> vo;
	
	printf_s("INFO::[%d]캐릭터 접속 - X : [%f], Y : [%f], Z : [%f], Yaw : [%f]\n",		vo.SessionID, vo.X, vo.Y, vo.Z, vo.Yaw);
	
	EnterCriticalSection(&criticalSec);
	PlayerVO* pVO = &playerInfo.players[vo.SessionID];
	pVO << vo;
	LeaveCriticalSection(&criticalSec);
	SessionSocket[vo.SessionID] = pSocket->socket;

	printf_s("INFO::클라이언트 수 : %d\n", SessionSocket.size());
	
	BroadcastNewPlayer(vo);
}

void PacketProcess::ReceiveCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	PlayerVO vo;
	RecvStream >> vo;
	//printf_s("[INFO][%d]정보 수신 - %d\n", vo.SessionID, vo.PlayerCond);
	EnterCriticalSection(&criticalSec);

	PlayerVO* pinfo = &playerInfo.players[vo.SessionID];
	pinfo << vo;

	LeaveCriticalSection(&criticalSec);

	WriteCharactersInfoToSocket(pSocket);
	monsterIOCP->Send(pSocket);

	////몬스터 정보를 던져주자
	if (monInfo.monsters.size() == 0)
	{
		monInfo.InitializeMonsterSet();
	}

	//몬스터 정보를 던져주자
	stringstream SendMonsterStream;
	SendMonsterStream << EPacketType::SYNC_MONSTER << endl;
	SendMonsterStream << monInfo << endl;

	Broadcast(SendMonsterStream);
}

void PacketProcess::Logout(stringstream& RecvStream, stSOCKETINFO* pSocket)
{

}

void PacketProcess::BroadcastNewPlayer(PlayerVO vo)
{
	stringstream SendStream;

	SendStream << EPacketType::ENTER_NEW_PLAYER << endl;
	SendStream << vo << endl;
	
	Broadcast(SendStream);
}

void PacketProcess::Broadcast(stringstream& SendStream)
{
	stSOCKETINFO* client = new stSOCKETINFO;
	for (const auto& kvp : SessionSocket)
	{
		client->socket = kvp.second;
		CopyMemory(client->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		client->dataBuf.buf = client->messageBuffer;
		client->dataBuf.len = SendStream.str().length();

		monsterIOCP->Send(client);
	}
}



void PacketProcess::WriteCharactersInfoToSocket(stSOCKETINFO* pSocket)
{
	//왜인지 모르겠지만 잔여 데이터 때문에 클리어해줘야함
	stringstream SendStream;
	// 직렬화	
	SendStream << EPacketType::RECV_PLAYER << endl;
	SendStream << playerInfo << endl;

	// !!! 중요 !!! data.buf 에다 직접 데이터를 쓰면 쓰레기값이 전달될 수 있음
	CopyMemory(pSocket->messageBuffer, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pSocket->dataBuf.buf = pSocket->messageBuffer;
	pSocket->dataBuf.len = SendStream.str().length();
}