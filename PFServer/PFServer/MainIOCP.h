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

	static void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);
private:

	friend class PacketProcesses;

	static CharacterInfo			cInfo;		// 접속한 클라이언트의 정보를 저장	
	static map<int, SOCKET> SessionSocket;		// 세션별 소켓 저장
	static float				 HitPoint;		// 타격 데미지
	static PlayerVO					   vo;		// 캐릭터 정보
	static DataAccess				  Dao;		// DataAccess 객체
	static CRITICAL_SECTION		csPlayers;		// CharactersInfo 임계영역

	FuncProcess		fnProcess[100];	// 패킷 처리 구조체
};