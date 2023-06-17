// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "CharacterStatus.h"

#include "GInst.h"
#include "Kismet/GameplayStatics.h"

#include "User.h"
#include "UserCntrl.h"

#include "PlayerHUDWidget.h"

#include "UserPlayMode.h"


AUserPlayMode::AUserPlayMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = AUser::StaticClass();
	PlayerControllerClass = AUserCntrl::StaticClass();

	//로그인 위젯 호출
	GetClassAsset(HUDWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/MainPlay/BPW_PlayerHUDWidget.BPW_PlayerHUDWidget_C'");

	//스탯 위젯 호출
	GetClassAsset(StatusWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/Sub/BPW_CharStatus.BPW_CharStatus_C'");

	isOnStatusWidget = false;
}

void AUserPlayMode::BeginPlay()
{
	Super::BeginPlay();

}

void AUserPlayMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUserPlayMode::SetPlayerHUDWidget()
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	IS_NULL(CurrentWidget);
	CurrentWidget->AddToViewport();
}

void AUserPlayMode::ShowMonsterHPBar(bool bShow)
{
	Cast<UPlayerHUDWidget>(CurrentWidget)->ShowMonsterHPBar(bShow);
}

void AUserPlayMode::SetMonsterHPRatio(float r)
{
	Cast<UPlayerHUDWidget>(CurrentWidget)->SetMonsterHPRatio(r);
}

void AUserPlayMode::SetPlayerStatusInfo(PlayerVO* ptrPVO)
{
	Cast<UPlayerHUDWidget>(CurrentWidget)->SetPlayerStatusInfo(ptrPVO);
}

void AUserPlayMode::SetChatMsgAtWindow(const string* msg)
{
	Cast<UPlayerHUDWidget>(CurrentWidget)->SetChatMsgAtWindow(msg);
}

void AUserPlayMode::ShowPlayerStatusWidget()
{
	IS_NULL(StatusWidgetClass);

	//스탯 정보 설정
	if (isOnStatusWidget)
	{
		CharacterStatusWidget->RemoveFromViewport();
		isOnStatusWidget = false;
	}
	else
	{
		CharacterStatusWidget = CreateWidget<UUserWidget>(GetWorld(), StatusWidgetClass);
		CharacterStatusWidget->AddToViewport();
		isOnStatusWidget = true;
	}
}


