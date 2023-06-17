#pragma once
#include "IOCPBase.h"
#include "PlayerVO.h"
#include "MonsterVO.h"
#include "PacketProcess.h"
//서버 포트
#define SERVER_PORT		8001

using namespace std;

class MonsterIOCP : public IOCPBase
{
public:
	MonsterIOCP();
	virtual ~MonsterIOCP();

	virtual bool Initialize() override;

	virtual void StartServer() override;
	// 작업 스레드 생성
	virtual bool CreateWorkerThread() override;
	// 작업 스레드
	virtual void WorkerThread() override;
	// 클라이언트에게 송신
	static void Send(stSOCKETINFO* pSocket);


	//static void WriteCharactersInfoToSocket(stSOCKETINFO* pSocket);
	//static void Broadcast(stringstream& SendStream);
	//void InitializeMonsterSet();
	//void MonsterMainThread();

protected:


private:
	static PacketProcess	    packetProc;		// 패킷 처리 클래스

	//static map<int, SOCKET>  SessionSocket;		// 세션별 소켓 저장
	//static CharacterInfo	 	     cInfo;		// 접속한 클라이언트의 정보를 저장	

	//static MonsterSet			   monInfo;		// 몬스터 정보 저장
	//static CRITICAL_SECTION	   criticalSec;		// CharactersInfo 임계영역

};

