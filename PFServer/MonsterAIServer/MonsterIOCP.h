//#pragma once
//#include "IOCPBase.h"
//#include "PlayerVO.h"
//#include "MonsterVO.h"
//#include "PacketProcess.h"
////서버 포트
//#define SERVER_PORT		8001
//
//using namespace std;
//
//class MonsterIOCP : public IOCPBase
//{
//public:
//	MonsterIOCP();
//	virtual ~MonsterIOCP();
//
//	virtual bool Initialize() override;
//
//	virtual void StartServer() override;
//	// 작업 스레드 생성
//	virtual bool CreateWorkerThread() override;
//	// 작업 스레드
//	virtual void WorkerThread() override;
//	// 클라이언트에게 송신
//	static void Send(stSOCKETINFO* pSocket);
//
//protected:
//
//
//private:
//	static PacketProcess	    packetProc;		// 패킷 처리 클래스
//};
//
