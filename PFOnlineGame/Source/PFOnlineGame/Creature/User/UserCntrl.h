// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserNetworkCntrl.h"

#include "UserCntrl.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AUserCntrl : public AUserNetworkCntrl
{
	GENERATED_BODY()
public:
	AUserCntrl();

protected:
	//플레이어와 빙의 시 호출 되는 함수(있어야 빙의 된다)
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void BeginPlay() override;


	void GetMouseButtonDown();
	void GetMouseButtonUp();
	void DoLeftClickAction();

	void ShowMonsterInfo();

	void ShowCharacterStatus();
	
	bool isClick = false;

	AActor* aClickedMonster;
};
