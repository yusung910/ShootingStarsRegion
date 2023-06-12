// Copyright Epic Games, Inc. All Rights Reserved.
#include "PFOnlineGame.h"

#include "Alert.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Confirm.h"

#include "GInst.h"

#include "Kismet/GameplayStatics.h"

#include "Login.h"
#include "CharSelectCamSplineA.h"

#include "User.h"
#include "UserCntrl.h"

#include "MainGMB.h"

AMainGMB::AMainGMB()
{
	PrimaryActorTick.bCanEverTick = true;

	//alert 위젯 호출
	GetClassAsset(AlertWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/Message/BPW_Alert.BPW_Alert_C'");

	//confirm 위젯
	GetClassAsset(ConfirmWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/Message/BPW_Confirm.BPW_Confirm_C'");

}

void AMainGMB::BeginPlay()
{
	Super::BeginPlay();
	gi = Cast<UGinst>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void AMainGMB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMainGMB::ShowAlert(FString msg)
{
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
	AlertWidget = CreateWidget<UUserWidget>(GetWorld(), AlertWidgetClass); //위젯 생성

	if (AlertWidget->IsInViewport())
		return; 

	AlertWidget->AddToViewport();
	
	Cast<UAlert>(AlertWidget)->SetMessageCopy(msg);
}

void AMainGMB::SetMessageCopy(FString msg)
{
	Cast<UAlert>(AlertWidget)->SetMessageCopy(msg);
}

void AMainGMB::SetVisibleBtnConfirm(bool b)
{
	Cast<UAlert>(AlertWidget)->SetVisibleBtnConfirm(b);
}

void AMainGMB::ShowConfirm(FString msg)
{
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
	ConfirmWidget = CreateWidget<UUserWidget>(GetWorld(), ConfirmWidgetClass); //위젯 생성

	ConfirmWidget->AddToViewport();
	Cast<UAlert>(ConfirmWidget)->SetMessageCopy(msg);
}

void AMainGMB::CloseAlert()
{
	if (AlertWidget != nullptr)
	{
		AlertWidget->RemoveFromViewport();
	}

	if (ConfirmWidget != nullptr)
	{
		ConfirmWidget->RemoveFromViewport();
	}

	if (deleAlertBtn.IsBound())
	{
		deleAlertBtn.Broadcast();
	}
}

void AMainGMB::ConfirmYes()
{
	if (deleConfirmBtn.IsBound())
	{
		deleConfirmBtn.Broadcast();
	}
}


void AMainGMB::GameEnd()
{
	//Socket->Logout();
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}

void AMainGMB::ShowErrorMsg(EServerStatusCode code)
{
	switch (code)
	{
	case EServerStatusCode::FAIL:
	case EServerStatusCode::SOCKET_CREATE_FAIL:
	case EServerStatusCode::SOCKET_CONNECT_FAIL:
	case EServerStatusCode::PACKET_SEND_FAIL:
	case EServerStatusCode::PACKET_RECEIVE_FAIL:
	case EServerStatusCode::DISCONNECTED:
		ShowAlert(UTF8_TO_TCHAR("서버가 닫혔습니다."));
		deleAlertBtn.AddLambda([this] {
			GameEnd();
		});
		break;
	case EServerStatusCode::AUTHORIZED_FAIL:
		ShowAlert(UTF8_TO_TCHAR("ID 또는 비밀번호가 잘못되었습니다, 확인하여주세요."));
		break;
	}
}

bool AMainGMB::CheckUpServerCorrespondenceStatus(EServerStatusCode code)
{
	bool bRet = false;
	switch (code)
	{
	case EServerStatusCode::CONNECTED:
	case EServerStatusCode::SUCCESS:
	case EServerStatusCode::PACKET_SEND_SUCCESS:
	case EServerStatusCode::PACKET_RECEIVE_SUCCESS:
		bRet = true;
		break;
	case EServerStatusCode::FAIL:
	case EServerStatusCode::SOCKET_CREATE_FAIL:
	case EServerStatusCode::SOCKET_CONNECT_FAIL:
	case EServerStatusCode::PACKET_SEND_FAIL:
	case EServerStatusCode::PACKET_RECEIVE_FAIL:
	case EServerStatusCode::AUTHORIZED_FAIL:
	case EServerStatusCode::DISCONNECTED:
		bRet = false;
		break;
	}

	return bRet;
}

void AMainGMB::StartMainGamePlay(PlayerVO vo)
{
	//게임 시작하기 전 플레이 할 캐릭터의 정보를 GameInstance에 저장한다
	gi->SetGamePlayerCharVO(vo);
	//게임 시작!!
	UGameplayStatics::OpenLevel(GetWorld(), "FrozenCove");
}
