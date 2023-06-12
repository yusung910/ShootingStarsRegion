// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "CharManagementGMB.h"
#include "MainGMB.h"

#include "CharacterCreate.h"


void UCharacterCreate::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//게임모드 세팅
	gmode = Cast<ACharManagementGMB>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGMode = Cast<AMainGMB>(gmode);

	btnKnight->OnClicked.AddDynamic(this, &UCharacterCreate::DoBtnKnightClick);
	btnGunner->OnClicked.AddDynamic(this, &UCharacterCreate::DoBtnGunnerClick);
	btnCreate->OnClicked.AddDynamic(this, &UCharacterCreate::CreateCharacter);
	btnBack->OnClicked.AddDynamic(this, &UCharacterCreate::MoveToBackDp);

	classBtns.Add("btnKnight", btnKnight);
	classBtns.Add("btnGunner", btnGunner);
}

void UCharacterCreate::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterCreate::SetCreateCharacterPawn()
{
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this]
	{
		txtCharNm->SetIsReadOnly(false);
		txtCharNm->SetFocus();
		SetSelectedBtnColor();
		gmode->SpawnCharActorInCreateCharacter(CLASS_SEQ);
	});

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, .2f, false);
}

void UCharacterCreate::DoBtnKnightClick()
{
	CLASS_SEQ = "1";
	SetCreateCharacterPawn();
}

void UCharacterCreate::DoBtnGunnerClick()
{
	CLASS_SEQ = "2";
	SetCreateCharacterPawn();
}

void UCharacterCreate::MoveToBackDp()
{
	gmode->MoveCamCharSelectFromCreate();
}

void UCharacterCreate::CreateCharacter()
{
	if (CLASS_SEQ.IsEmpty())
	{
		MainGMode->ShowAlert(UTF8_TO_TCHAR("클래스 선택하여주세요."));
	}
	else if (txtCharNm->GetText().IsEmpty())
	{
		MainGMode->ShowAlert(UTF8_TO_TCHAR("캐릭터 이름을 입력하여주세요."));
		MainGMode->deleAlertBtn.AddLambda([this] {
			txtCharNm->SetFocus();
		});
	}
	else
	{
		PlayerVO vo;
		vo.CHAR_NM = TCHAR_TO_UTF8(*txtCharNm->GetText().ToString());
		vo.CLASS_SEQ = TCHAR_TO_UTF8(*CLASS_SEQ);
		gmode->CreateCharacter(vo);
		MoveToBackDp();
	}
}

void UCharacterCreate::SetSelectedBtnColor()
{
	for (auto btn : classBtns)
	{
		if (btn.Value != nullptr)
		{
			btn.Value->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		}
	}

	if (CLASS_SEQ.Equals("1"))
	{
		classBtns["btnKnight"]->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	}
	else if (CLASS_SEQ.Equals("2"))
	{
		classBtns["btnGunner"]->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	}
}
