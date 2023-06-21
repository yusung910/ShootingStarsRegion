//#pragma once
//#include "stdafx.h"
//class MonsterIOCP;
//class PlayerVO;
//
//class PacketProcess
//{
//public:
//	PacketProcess();
//	~PacketProcess();
//
//	//캐릭터 접속
//	void EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
//
//	//캐릭터 정보가 담긴 패킷 수신 함수
//	void ReceiveCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);
//
//	//몬스터 피격 패킷 수신함수
//	void HitMonster(stringstream& RecvStream, stSOCKETINFO* pSocket);
//
//	//플레이어 피격 패킷 수신함수
//	void HitPlayer(stringstream& RecvStream, stSOCKETINFO* pSocket);
//
//	//캐릭터 로그아웃
//	void Logout(stringstream& RecvStream, stSOCKETINFO* pSocket);
//
//	//새로 접속한 캐릭터 전송
//	void BroadcastNewPlayer(PlayerVO vo);
//
//	//접속한 모든 유저들에게 패킷을 전송
//	void Broadcast(stringstream& SendStream);
//
//	//접속한 유저들의 캐릭터 정보를 전송하는 함수
//	void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);
//
//	//monster 서버의 iocp 포인터 변수를 저장한다.
//	void SetMonsterIOCP(MonsterIOCP* iocp) { monsterIOCP = iocp; }
//
//private:
//	MonsterIOCP* monsterIOCP;
//	static map<int, SOCKET> SessionSocket;	// 세션별 소켓 저장
//	static CharacterInfo playerInfo;		// 접속한 클라이언트의 정보를 저장	
//
//	static MonsterSet monInfo;				// 몬스터 정보 저장
//	static CRITICAL_SECTION	criticalSec;	// CharactersInfo 임계영역
//
//};
//
