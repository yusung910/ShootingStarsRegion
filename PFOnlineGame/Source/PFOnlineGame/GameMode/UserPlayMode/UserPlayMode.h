// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/MainGMB.h"

#include "ClientSocket.h"
#include "UserPlayMode.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AUserPlayMode : public AMainGMB
{
	GENERATED_BODY()

public:
	AUserPlayMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetPlayerHUDWidget();

	void ShowPlayerStatusWidget();

	void ShowMonsterHPBar(bool bShow);

	void SetMonsterHPRatio(float r);

	void SetPlayerStatusInfo(PlayerVO* ptrPVO);

	void SetChatMsgAtWindow(const string* msg);

	UPROPERTY()
		TSubclassOf<class UUserWidget> HUDWidgetClass; //플레이어 메인 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> StatusWidgetClass; //플레이어 메인 위젯
private:
	UPROPERTY()
		class UUserWidget* CharacterStatusWidget;


	bool isOnStatusWidget;
};
