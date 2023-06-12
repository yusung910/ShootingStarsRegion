// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "MainGMB.h"

#include "Alert.h"

void UAlert::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gmode = Cast<AMainGMB>(UGameplayStatics::GetGameMode(GetWorld()));
	btnConfirm->OnClicked.AddDynamic(this, &UAlert::CloseAlert);
	bIsFocusable = true;
	
}

void UAlert::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	focusTimer += InDeltaTime;
	if (focusTimer >= 0.02f && !isSetFocus)
	{
		isSetFocus = true;
		SetFocus();
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape))
	{
		gmode->CloseAlert();
	}
}

void UAlert::SetMessageCopy(FString message)
{
	TxtContent->SetText(UKismetTextLibrary::Conv_StringToText(message));
}

void UAlert::SetVisibleBtnConfirm(bool bFlag)
{
	ESlateVisibility visibleFlag = (bFlag) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	btnConfirm->SetVisibility(visibleFlag);
}

void UAlert::CloseAlert()
{
	gmode->CloseAlert();
}
