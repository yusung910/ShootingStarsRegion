// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

#include "Ginst.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "Math/UnrealMathUtility.h"

#include "UserPlayMode.h"

#include "CharacterStatus.h"

void UCharacterStatus::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerVO LocalPVo = Cast<UGinst>(GetWorld()->GetGameInstance())->GetGamePlayerCharVO();

	txtCharNm->SetText(FText::FromString(FString(UTF8_TO_TCHAR(LocalPVo.CHAR_NM.c_str()))));
	txtClassNm->SetText(FText::FromString(FString(UTF8_TO_TCHAR(LocalPVo.CLASS_NM.c_str()))));
	txtLevel->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.CHAR_LVL)));

	txtStr->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.STR)));
	txtDex->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.DEX)));
	txtVital->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.VITAL)));
	txtEnergy->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.ENERGY)));

	txtCurExp->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.CUR_EXP)));
	txtReqExp->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.REQ_EXP)));

	txtMaxHp->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.MAX_HP)));
	txtMaxMana->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.MAX_MANA)));
	txtDamage->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), LocalPVo.DAMAGE)));
	txtAgilityRate->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), FMath::RoundHalfFromZero(LocalPVo.DODGE_RATE))));
	txtAccuracyRate->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), FMath::RoundHalfFromZero(LocalPVo.ACCURACY_RATE))));


	PB_ExpRatio->SetPercent(LocalPVo.CUR_EXP / LocalPVo.REQ_EXP);

	btnPopupClose->OnClicked.AddDynamic(this, &UCharacterStatus::CloseStatusWidget);
	btnStrUp->OnClicked.AddDynamic(this, &UCharacterStatus::IncreaseSTR);
	btnDexUp->OnClicked.AddDynamic(this, &UCharacterStatus::IncreaseDEX);
	btnVitalUp->OnClicked.AddDynamic(this, &UCharacterStatus::IncreaseVITAL);
	btnEnergyUp->OnClicked.AddDynamic(this, &UCharacterStatus::IncreaseENERGY);

}

void UCharacterStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterStatus::SetCharacterStatus()
{

	
}

void UCharacterStatus::CloseStatusWidget()
{
	Cast<AUserPlayMode>(UGameplayStatics::GetGameMode(GetWorld()))->ShowPlayerStatusWidget();
}

void UCharacterStatus::IncreaseSTR()
{
	LOG_SCREEN("IncreaseSTR");
}

void UCharacterStatus::IncreaseDEX()
{
	LOG_SCREEN("IncreaseDEX");
}

void UCharacterStatus::IncreaseVITAL()
{
	LOG_SCREEN("IncreaseVITAL");
}

void UCharacterStatus::IncreaseENERGY()
{
	LOG_SCREEN("IncreaseENERGY");
}
