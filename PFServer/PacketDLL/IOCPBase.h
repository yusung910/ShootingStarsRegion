#pragma once

#include "stdafx.h"

// winsock2 사용을 위해 아래 코멘트 추가
#pragma comment(lib, "ws2_32.lib")


class IOCPBase
{
public:
	PACKETDLL_API IOCPBase();
	PACKETDLL_API virtual ~IOCPBase();

	// 소켓 등록 및 서버 정보 설정
	PACKETDLL_API virtual bool Initialize();
	// 서버 시작
	PACKETDLL_API virtual void StartServer();
	// 작업 스레드 생성
	PACKETDLL_API virtual bool CreateWorkerThread() { return false; };
	// 작업 스레드
	PACKETDLL_API virtual void WorkerThread() {};
	// 클라이언트에게 송신
	PACKETDLL_API virtual void Send(stSOCKETINFO* pSocket);
	// 클라이언트 수신 대기
	PACKETDLL_API virtual void Recv(stSOCKETINFO* pSocket);

protected:
	stSOCKETINFO*	SocketInfo;		// 소켓 정보
	SOCKET			ListenSocket;	// 서버 리슨 소켓
	HANDLE			hIOCP;			// IOCP 커널 객체 핸들
	bool			bAccept;		// 요청 동작 플래그
	bool			bWorkerThread;	// 작업 스레드 동작 플래그
	HANDLE*			hWorkerHandle;	// 작업 스레드 핸들		
	int				nThreadCnt;


};