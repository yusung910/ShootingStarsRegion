#pragma once

#include "IOCPBase.h"
#include "DataAccess.h"
#include "PlayerVO.h"
#include "MonsterVO.h"

//서버 포트
#define SERVER_PORT		8000

using namespace std;

class MainIOCP : public IOCPBase
{
public:
	MainIOCP();
	virtual ~MainIOCP();

	virtual bool Initialize() override;

	virtual void StartServer() override;
	// 작업 스레드 생성
	virtual bool CreateWorkerThread() override;
	// 작업 스레드
	virtual void WorkerThread() override;
	// 클라이언트에게 송신
	static void Send(stSOCKETINFO* pSocket);

	// 캐릭터 생성처리
	static void CreateCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 계정 로그인
	static void Login(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 목록 조회
	static void SearchCharacters(stringstream& RecvStream, stSOCKETINFO* pSocket);

	// 캐릭터 캐릭터 삭제처리
	static void DeleteCharacter(stringstream& RecvStream, stSOCKETINFO* pSocket);

private:
	static PlayerVO					   vo;		// 캐릭터 정보
	static DataAccess				  Dao;		// DataAccess 객체
	static CRITICAL_SECTION		csPlayers;		// CharactersInfo 임계영역

	FuncProcess		fnProcess[EPacketType::PACKET_TOT_AMOUNT];	// 패킷 처리 구조체
};