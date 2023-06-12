// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "MainGMB.h"
#include "CharManagementGMB.h"

#include "CharacterRow.h"

void UCharacterRow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//게임모드 세팅
	gmode = Cast<ACharManagementGMB>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGMode = Cast<AMainGMB>(gmode);

	btnStart->OnClicked.AddDynamic(this, &UCharacterRow::CharacterPreviewChange);
	btnDelete->OnClicked.AddDynamic(this, &UCharacterRow::DeleteCharacterConfirm);
}


void UCharacterRow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isStartClicked)
	{
		dblTermTimer += InDeltaTime;
		if (dblTermTimer > 3.0f)
		{
			dblTermTimer = 0.0f;
			isStartClicked = false;
			gmode->SetSelectedCharSeq(FString());
		}
	}
}

void UCharacterRow::SetCharinfo(PlayerVO vo)
{
	txtCharName->SetText(FText::FromString(FString(UTF8_TO_TCHAR(vo.CHAR_NM.c_str()))));
	txtCharLvl->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), vo.CHAR_LVL)));
	txtCharClass->SetText(FText::FromString(FString(UTF8_TO_TCHAR(vo.CLASS_NM.c_str()))));
	charVO = vo;
}

void UCharacterRow::CharacterPreviewChange()
{
	isStartClicked = true;
	gmode->InitCharSelectBtnColor();
	SetColorAndOpacity(FLinearColor(0, 1, 0.1f, 1));
	gmode->SpawnUserActorInCharacterSelect(charVO);
	FString charSEQ = FString(charVO.CHAR_SEQ.c_str());
	if (dblTermTimer <= 3.0f &&
		charSEQ.Equals(gmode->GetSelectedCharSeq()))
	{
		//게임 시작
		gmode->StartMainGamePlay(charVO);
	}
	else
	{
		dblTermTimer = 0.0f;
		gmode->SetSelectedCharSeq(FString(charVO.CHAR_SEQ.c_str()));
	}
}

void UCharacterRow::DeleteCharacterConfirm()
{
	MainGMode->ShowConfirm(UTF8_TO_TCHAR("주의! 캐릭터를 삭제하시겠습니까?\n복구 되지 않습니다."));
	gmode->SetCharListWidgetEnable(false);
	//yes 버튼 눌렀을 때
	MainGMode->deleConfirmBtn.AddLambda([this] {
		gmode->DeleteCharacter(charVO);
		gmode->ShowCharacterSelectWidget();
		MainGMode->CloseAlert();
		gmode->SetCharListWidgetEnable(true);
	});

	//no 버튼 눌렀을 때
	MainGMode->deleAlertBtn.AddLambda([this] {
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		gmode->SetCharListWidgetEnable(true);
	});
}


void UCharacterRow::InitSelectBtnColor()
{
	SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
}