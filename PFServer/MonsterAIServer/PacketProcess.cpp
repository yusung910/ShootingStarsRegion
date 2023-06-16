#include "PacketProcess.h"
#include "PlayerVO.h"

PacketProcess::PacketProcess()
{
	printf_s("INFO::PacketController 생성\n");
}

PacketProcess::~PacketProcess()
{
	printf_s("INFO::PacketController 소멸\n");

}

void PacketProcess::EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
	printf_s("INFO::[Receive Packet - 'ENROLL_PLAYER']");

	PlayerVO vo;
	RecvStream >> vo;
	
	//printf_s("INFO::[%d]캐릭터 접속 - X : [%f], Y : [%f], Z : [%f], Yaw : [%f]\n",		vo.SessionID, vo.X, vo.Y, vo.Z, vo.Yaw);
	
	//EnterCriticalSection(&MainIOCP::csPlayers);
	//
	//PlayerVO* pVO = &MainIOCP::cInfo.players[vo.SessionID];
	//
	//pVO << vo;
	//
	//LeaveCriticalSection(&MainIOCP::csPlayers);
	//
	//MainIOCP::SessionSocket[vo.SessionID] = pSocket->socket;
	//
	//printf_s("INFO::클라이언트 수 : %d\n", MainIOCP::SessionSocket.size());
	//
	//BroadcastNewPlayer(vo);
}

void PacketProcess::Logout(stringstream& RecvStream, stSOCKETINFO* pSocket)
{
}

void PacketProcess::BroadcastNewPlayer(PlayerVO vo)
{
	stringstream SendStream;

	SendStream << EPacketType::ENTER_NEW_PLAYER << endl;
	SendStream << vo << endl;

	//Broadcast(SendStream);
}