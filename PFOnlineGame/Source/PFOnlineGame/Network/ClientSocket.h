// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SktCorresCmn.h"

#include "Runtime/Core/Public/HAL/Runnable.h"


/**
 * 
 */
class PFONLINEGAME_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	// 소켓 등록 및 설정
	EServerStatusCode InitSocket();
	// 서버와 연결
	EServerStatusCode Connect(const char* pszIP, int nPort);

	//로그아웃
	void Logout(const int& SessionId);

	void SetGameMode(AGameModeBase* gm);

	void CloseSocket();

	vector<map<string, string>> SetParseParam(string p);
	void SetPlayerController(AUserNetworkCntrl* cntrl);

	CharacterInfo* RecvCharacterInfo(stringstream& RecvStream);
	PlayerVO* RecvNewPlayer(stringstream& RecvStream);


	MonsterSet* RecvMonsterSet(stringstream& RecvStream);
	MonsterVO* RecvMonster(stringstream& RecvStream);

	string strChat;
	string* RecvChat(stringstream& RecvStream);

	// FRunnable Thread members	
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	// FRunnable override 함수
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();

	// 스레드 시작 및 종료
	EServerStatusCode StartListen();
	void StopListen();

	bool GetThreadStatus();
protected:
	SOCKET	ServerSocket;				// 서버와 연결할 소켓	
	char 	recvBuffer[MAX_BUFFER];		// 수신 버퍼 스트림	
	
	string UserSeq;

	static PlayerVO cinfo;

	//게임모드
	AGameModeBase* gmode;
	AUserPlayMode* UserPMode;
	AUserNetworkCntrl* userNetworkCntrl;

	//
	CharacterInfo info;
	PlayerVO NewPlayer;

	//
	MonsterSet MonsterInfoSet;
	MonsterVO MonsVO;
	//

};
