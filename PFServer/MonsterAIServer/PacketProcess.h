#pragma once
#include "stdafx.h"
class MonsterIOCP;
class PlayerVO;

class PacketProcess
{
public:
	PacketProcess();
	~PacketProcess();

	//캐릭터 접속
	void EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//
	void ReceiveCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//캐릭터 로그아웃
	void Logout(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//새로 접속한 캐릭터 전송
	void BroadcastNewPlayer(PlayerVO vo);
	
	//
	void SetMonsterIOCP(MonsterIOCP* iocp) { monsterIOCP = iocp; }

	//
	void Broadcast(stringstream& SendStream);

	//
	void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);

private:
	MonsterIOCP* monsterIOCP;
	static map<int, SOCKET> SessionSocket;	// 세션별 소켓 저장
	static CharacterInfo playerInfo;		// 접속한 클라이언트의 정보를 저장	

	static MonsterSet monInfo;				// 몬스터 정보 저장
	static CRITICAL_SECTION	criticalSec;	// CharactersInfo 임계영역

};

