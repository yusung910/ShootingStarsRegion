#pragma once
#ifdef PACKETDLL_EXPORTS
#define PACKETDLL_API __declspec(dllexport)
#else
#define PACKETDLL_API __declspec(dllimport)
#endif

// 멀티바이트 집합 사용시 define
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

//패킷 최대 버퍼
#define	MAX_BUFFER		4096
//서버 포트
#define SERVER_PORT		8000
//최대 클라이언트 연결 수
#define MAX_CLIENTS		100


enum PACKETDLL_API EPacketType
{
	LOGIN,
	SEARCH_CHARACTER,
	LOGOUT,
	CREATE_CHARACTER,
	DELETE_CHARACTER,
	ENROLL_PLAYER,
	SEND_PLAYER,
	ENTER_NEW_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	SYNC_MONSTER,
	RESPAWN_MONSTER,
	HIT_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER,
	DAMAGED_MONSTER,
	PACKET_TOT_AMOUNT
};

enum PACKETDLL_API ECondition
{
	IS_DEATH,
	IS_DEATHED,
	IS_DAMAGED,
	IS_ATTACK,
	IS_MOVE,
	IS_IDLE,
	IS_DASH,
};

enum PACKETDLL_API EDamagedType
{
	IS_NORMAL,
	IS_DOWN,
	IS_LEVITATION,
	IS_SLOW,
	IS_PARALYSIS,
	IS_POSION,
	IS_FLAME,
};


// IOCP 소켓 구조체
struct PACKETDLL_API stSOCKETINFO
{
	//구조체 주소, 전송 완료 확인을 위한 용도
	WSAOVERLAPPED	overlapped;

	//소켓 데이터의 송수신에서 데이터를 저장하고 보내기 위해 사용되는 버퍼
	WSABUF			dataBuf;

	//참조 소켓 인스턴스들에 사용되는 새로운 타입 //uint_ptr
	SOCKET			socket;

	//메세지 버퍼 저장 변수
	char			messageBuffer[MAX_BUFFER];

	//송신 버퍼 사이즈
	int				recvBytes;
	//수신 버퍼 사이즈
	int				sendBytes;
};

// 패킷 처리 함수 포인터
struct PACKETDLL_API FuncProcess
{
	void(*funcProcessPacket)(stringstream& RecvStream, stSOCKETINFO* pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};