// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"
//#include "UserCntrl.h"

#include "UserNetworkCntrl.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AUserNetworkCntrl : public APlayerController
{
	GENERATED_BODY()

public:
	AUserNetworkCntrl();

	void RecvWorldInfo(CharacterInfo* info);

	void RecvNewPlayer(PlayerVO* vo);

	void RecvMonsterSet(MonsterSet* monsterSet);

	void RecvSyncMonster(MonsterVO* _MonVO);

	MonsterSet* SendMonsterSet();

	void RecvSpawnMonster(MonsterVO* _MonVO);

	void RecvDestroyMonster(MonsterVO* _MonVO);

	void SendPlayerInfo();

	void UpdateNewPlayer();

	bool UpdateWorldInfo();

	void UpdatePlayerInfo(const PlayerVO& vo);

	void DestroyMonster();

	void UpdateMonsterSet();

	void SetServerConnected(EServerStatusCode code);

	void RecvChatMsg(const string* chat);

	void UpdateToChatMsgWindow();

	void StartMainGamePlay(PlayerVO vo);
protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle SendPlayerInfoHandle;	// 동기화 타이머 핸들러

	int SessionID;

	//새 플레이어 입장
	int nPlayers;
	bool bNewPlayerEntered;
	float fNewPlayerEnterTick;

	//몬스터 리젠 시작
	float fMonsterSpawnStart;

	//몬스터 수
	int nMonsters;

	//몬스터 생성
	bool isSpawnMonster;
	//몬스터 제거
	bool isDestroyMonster;

	CharacterInfo* OtherInfos; //다른 캐릭터 정보
	PlayerVO* pVO;	//서버에서 받아오는 캐릭터 정보

	MonsterSet* MonsterInfos;
	MonsterVO* MonsVO;
	
	EServerStatusCode isServerConnected;

	bool isServerDisconnected;

	bool isRecvChat;

	const string* sRecvChatMsg;

	bool isShowedAlert;

protected:
	UPROPERTY()
		class AUser* UserCharacter;

	UPROPERTY()
		class UGinst* gi;

	UPROPERTY()
		class AUserPlayMode* gmode;

	UPROPERTY()
		class AMainGMB* MainGmode;

	//소환된 몬스터를 저장
	TArray<AActor*> SpawnedMonsters;

};
