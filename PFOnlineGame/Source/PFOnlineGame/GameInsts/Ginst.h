// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SktCorresCmn.h"
#include "CorrespondencePacket.h"

#include "Ginst.generated.h"

DECLARE_MULTICAST_DELEGATE(FDeleCorrespondenceDone)
/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UGinst : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGinst();

	//EServerStatusCode InitSocket();

	//EServerStatusCode Connect(const char* pszIP, int nPort);

	//EServerStatusCode Login(const FText& Id, const FText& Pw);

	//EServerStatusCode SearchCharacters();

	//EServerStatusCode CreateCharacter(PlayerVO vo);

	//EServerStatusCode DeleteCharacter(PlayerVO vo);

	//EServerStatusCode EnrollPlayer(PlayerVO vo);

	//EServerStatusCode SendPlayer(PlayerVO vo);

	//EServerStatusCode HitPlayer(PlayerVO vo);

	//EServerStatusCode HitMonster(MonsterVO monsVO);

	//EServerStatusCode StartListen();

	//EServerStatusCode SendChatMessage(const string& msg);

	//void Disconnect();

	//void SetGameModeToClientSocket(AGameModeBase* gm);

	//void SetUserControllerToClientSocket(AUserNetworkCntrl* cntrl);

	//virtual void Init() override;

	//FDeleCorrespondenceDone DeleCorresDone;

	//void SetPlayerVOInfo(PlayerVO& dest, class AUser* depart);
	//void SetPlayerVOInfo(PlayerVO& dest, PlayerVO* depart);

	//bool GetThreadStatus();

	//void SetGamePlayerCharVO(PlayerVO vo);
	//FORCEINLINE PlayerVO GetGamePlayerCharVO() { return gamePlayChar; }

	//FORCEINLINE void SetSessionID(int n) { SessionID = n; }

	
private:
	CorrespondencePacket* Socket;

	//PlayerVO gamePlayChar;

	//AUserNetworkCntrl에서 생성한 세션아이디를 GameInstance에 저장한다
	//플레이어객체 정보를 생성할 때 사용한다
	//int SessionID;
};
