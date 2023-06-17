// Fill out your copyright notice in the Description page of Project Settings.

#include "PFOnlineGame.h"

#include <sstream>
#include <algorithm>
#include <string>
#include <cstdlib> //rand(), srand()
#include <ctime> //time()

#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "CharManagementGMB.h"

#include "MainGMB.h"
#include "UserPlayMode.h"
#include "UserNetworkCntrl.h"
#include "ClientSocket.h"

//static 변수 선언
PlayerVO ClientSocket::cinfo;

ClientSocket::ClientSocket()
	:StopTaskCounter(0)
{
}

ClientSocket::~ClientSocket()
{
	delete Thread;
	Thread = nullptr;

	closesocket(ServerSocket);
	WSACleanup();
}

EServerStatusCode ClientSocket::InitSocket()
{
	WSADATA wsaData;
	// 윈속 버전을 2.2로 초기화
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0)
	{
		return EServerStatusCode::SOCKET_CREATE_FAIL;
	}

	// TCP 소켓 생성	
	ServerSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ServerSocket == INVALID_SOCKET)
	{
		return EServerStatusCode::SOCKET_CREATE_FAIL;
	}


	return EServerStatusCode::SUCCESS;
}


EServerStatusCode ClientSocket::Connect(const char* pszIP, int nPort)
{
	// 접속할 서버 정보를 저장할 구조체
	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// 접속할 서버 포트 및 IP
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);

	int nRet = connect(ServerSocket, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR)
	{
		return EServerStatusCode::SOCKET_CONNECT_FAIL;
	}

	return EServerStatusCode::CONNECTED;
}




void ClientSocket::Logout(const int& SessionId)
{
	// 서버에게 로그아웃시킬 캐릭터 정보 전송
	stringstream SendStream;
	SendStream << EPacketType::LOGOUT << endl;
	SendStream << SessionId << endl;

	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
	{
		return;
	}

	closesocket(ServerSocket);
	WSACleanup();
}

void ClientSocket::SetGameMode(AGameModeBase* gm)
{
	gmode = gm;
	UserPMode = Cast<AUserPlayMode>(gm);
}

void ClientSocket::CloseSocket()
{
	closesocket(ServerSocket);
	WSACleanup();
}

vector<map<string, string>> ClientSocket::SetParseParam(string p)
{
	vector<map<string, string>> rslt;
	istringstream paramStream(p);

	string rowBuffer;

	vector<string> row;
	while (getline(paramStream, rowBuffer, ','))
	{
		map<string, string> params;
		istringstream rowStream(rowBuffer);
		string colBuffer;
		while (getline(rowStream, colBuffer, '&'))
		{
			istringstream kvStream(colBuffer);
			string kvStrBuf;
			vector<string> kvVec;
			while (getline(kvStream, kvStrBuf, '='))
			{
				kvVec.push_back(kvStrBuf);
			}

			params.insert(pair<string, string>(kvVec[0], kvVec[1]));
		}

		rslt.push_back(params);
	}

	return rslt;
}

void ClientSocket::SetPlayerController(AUserNetworkCntrl* cntrl)
{
	if (cntrl != nullptr)
	{
		userNetworkCntrl = cntrl;
	}
}

CharacterInfo* ClientSocket::RecvCharacterInfo(stringstream& RecvStream)
{
	// 캐릭터 정보를 얻어 반환		
	RecvStream >> info;
	return &info;
}

PlayerVO* ClientSocket::RecvNewPlayer(stringstream& RecvStream)
{
	// 새 플레이어 정보를 얻어 반환
	RecvStream >> NewPlayer;
	return &NewPlayer;
}

MonsterSet* ClientSocket::RecvMonsterSet(stringstream& RecvStream)
{
	// 몬스터 집합 정보를 얻어 반환
	RecvStream >> MonsterInfoSet;
	return &MonsterInfoSet;
}

MonsterVO* ClientSocket::RecvMonster(stringstream& RecvStream)
{
	RecvStream >> MonsVO;
	return &MonsVO;
}

string* ClientSocket::RecvChat(stringstream& RecvStream)
{
	// 채팅 정보를 얻어 반환
	RecvStream >> strChat;
	replace(strChat.begin(), strChat.end(), '_', ' ');
	return &strChat;
}


bool ClientSocket::Init()
{
	return true;
}

uint32 ClientSocket::Run()
{
	// 초기 init 과정을 기다림
	FPlatformProcess::Sleep(0.03);
	// recv while loop 시작
	// StopTaskCounter 클래스 변수를 사용해 Thread Safety하게 해줌
	while (StopTaskCounter.GetValue() == 0 && userNetworkCntrl != nullptr)
	{
		stringstream RecvStream;
		int PacketType;
		int nRecvLen = recv(
			ServerSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0
		);
		if (nRecvLen > 0)
		{
			// 패킷 처리
			RecvStream << recvBuffer;
			RecvStream >> PacketType;

			userNetworkCntrl->SetServerConnected(EServerStatusCode::PACKET_RECEIVE_SUCCESS);
			
			switch (PacketType)
			{
			case EPacketType::RECV_PLAYER:
				userNetworkCntrl->RecvWorldInfo(RecvCharacterInfo(RecvStream));
				break;
			case EPacketType::ENTER_NEW_PLAYER:
				userNetworkCntrl->RecvNewPlayer(RecvNewPlayer(RecvStream));
				break;
			case EPacketType::SYNC_MONSTER:
				userNetworkCntrl->RecvMonsterSet(RecvMonsterSet(RecvStream));
				break;
			case EPacketType::DESTROY_MONSTER:
				userNetworkCntrl->RecvDestroyMonster(RecvMonster(RecvStream));
				break;
			case EPacketType::DAMAGED_MONSTER:
				userNetworkCntrl->RecvSyncMonster(RecvMonster(RecvStream));
				break;
			case EPacketType::CHAT:
				userNetworkCntrl->RecvChatMsg(RecvChat(RecvStream));
				break;

			}
		}
		else
		{
			//패킷 수신에 실패했을 경우
			userNetworkCntrl->SetServerConnected(EServerStatusCode::PACKET_RECEIVE_FAIL);
		}
	}
	return 0;
}

void ClientSocket::Stop()
{
	// thread safety 변수를 조작해 while loop 가 돌지 못하게 함
	StopTaskCounter.Increment();
}

void ClientSocket::Exit()
{
}

EServerStatusCode ClientSocket::StartListen()
{
	// 스레드 시작
	if (Thread != nullptr) return EServerStatusCode::FAIL;
	Thread = FRunnableThread::Create(this, TEXT("ClientSocket"), 0, TPri_BelowNormal);
	return (Thread != nullptr) ? EServerStatusCode::SUCCESS : EServerStatusCode::FAIL;
}

void ClientSocket::StopListen()
{
	// 스레드 종료
	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
		Thread = nullptr;
	}
	StopTaskCounter.Reset();
}

bool ClientSocket::GetThreadStatus()
{
	return (Thread == nullptr);
}