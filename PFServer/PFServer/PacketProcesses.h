#pragma once
#include "DataAccess.h"
using namespace std;

class PacketProcesses
{
public:
	PacketProcesses();

	// 계정 로그인
	static void Login(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 목록 조회
	static void SearchCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 로그아웃 처리
	static void Logout(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 생성처리
	static void CreateCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 캐릭터 삭제처리
	static void DeleteCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 접속한 플레이어의 캐릭터를 서버에 등록처리
	static void EnrollCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//sync player 접속한 플레이어들의 데이터 처리
	static void SyncCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//플레이어 피격 패킷 처리
	static void HitCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	////몬스터 히트 패킷 처리
	//static void HitMonster(stringstream& RecvStream, stSOCKETINFO* pSocket);

	////몬스터 sync 패킷 전송
	//static void SyncMonsters(int mId);

	////몬스터 소환 패킷 전송
	//static void SpawnAllMonsters();

	//채팅 메시지 전송
	static void BroadcastChat(stringstream& RecvStream, stSOCKETINFO* pSocket);

	//새로운 플레이어 패킷
	static void BroadcastNewPlayer(PlayerVO vo);

	// 브로드캐스트 함수
	static void Broadcast(stringstream& SendStream);

private:

};

