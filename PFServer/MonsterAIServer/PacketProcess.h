#pragma once
#include "stdafx.h"
class PlayerVO;

class PacketProcess
{
public:
	PacketProcess();
	~PacketProcess();

	void EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
	void Logout(stringstream& RecvStream, stSOCKETINFO* pSocket);

	void BroadcastNewPlayer(PlayerVO vo);

};

