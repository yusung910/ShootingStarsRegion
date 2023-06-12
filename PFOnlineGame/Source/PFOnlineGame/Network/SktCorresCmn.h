// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
// winsock2 사용을 위해 아래 코멘트 추가
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <string>

#include "CharacterInfo.h"
#include "MonsterSet.h"

class ACharManagementGMB;
class AGameBaseMode;
class AUserPlayMode;
class AUserCntrl;
class AUserNetworkCntrl;

using namespace std;

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
#define SERVER_IP		"127.0.0.1"
#define MAX_CLIENTS		100

// 소켓 통신 구조체
struct stSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[MAX_BUFFER];
	int				recvBytes;
	int				sendBytes;
};

// 패킷 정보
enum EPacketType
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
//

UENUM(BlueprintType)
enum class EServerStatusCode : uint8
{
	CONNECTED					UMETA(DisplayName = "CONNECTED"),
	SUCCESS						UMETA(DisplayName = "SUCCESS"),
	PACKET_SEND_SUCCESS			UMETA(DisplayName = "PACKET_SEND_SUCCESS"),
	PACKET_RECEIVE_SUCCESS		UMETA(DisplayName = "PACKET_RECEIVE_SUCCESS"),
	FAIL						UMETA(DisplayName = "FAIL"),
	SOCKET_CREATE_FAIL			UMETA(DisplayName = "SOCKET_CREATE_FAIL"),
	SOCKET_CONNECT_FAIL			UMETA(DisplayName = "SOCKET_CONNECT_FAIL"),
	PACKET_SEND_FAIL			UMETA(DisplayName = "PACKET_SEND_FAIL"),
	PACKET_RECEIVE_FAIL			UMETA(DisplayName = "PACKET_RECEIVE_FAIL"),
	AUTHORIZED_FAIL				UMETA(DisplayName = "AUTHORIZED_FAIL"),
	DISCONNECTED				UMETA(DisplayName = "DISCONNECTED"),
};

/**
 * 
 */
class PFONLINEGAME_API SktCorresCmn
{
public:
	SktCorresCmn();
	~SktCorresCmn();

private:

};
