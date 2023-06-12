// Fill out your copyright notice in the Description page of Project Settings.

#include "PFOnlineGame.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "ChatWindow.h"

#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "Ginst.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "PlayerHUDWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//게임모드 세팅
	gi = Cast<UGinst>(GetWorld()->GetGameInstance());


	deleChatInput.BindUFunction(this, "SendChatMsg");
	InputChat->OnTextCommitted.Add(deleChatInput);

	bIsFocusable = true;
}


void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* playerCntrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (playerCntrl->WasInputKeyJustReleased(EKeys::Enter)
		|| playerCntrl->WasInputKeyJustReleased(EKeys::Escape))
	{

		SetVisibleChatInput();
	}

}

void UPlayerHUDWidget::SetVisibleChatInput()
{
	ESlateVisibility visibleFlag = (ChatInputContainer->GetVisibility() == ESlateVisibility::Hidden) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	ChatInputContainer->SetVisibility(visibleFlag);

	if (visibleFlag == ESlateVisibility::Visible)
	{
		InputChat->ClearKeyboardFocusOnCommit = true;
		InputChat->SetFocus();
	}

}

void UPlayerHUDWidget::SetChatMsgAtWindow(const string* msg)
{
	if (msg->empty())
		return;

	UTextBlock* chatMsg = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	chatMsg->SetText(FText::FromString(FString(UTF8_TO_TCHAR(msg->c_str()))));
	chatQueue.Enqueue(chatMsg);
	SB_ChatLog->AddChild(chatMsg);
}

void UPlayerHUDWidget::SendChatMsg(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ChatInputContainer->SetVisibility(ESlateVisibility::Hidden);

		FTimerHandle timerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			timerHandle,
			FTimerDelegate::CreateLambda([this]() {
				//0.2초 후 포커스를 게임 뷰포트로 옮겨줘야
				//PlayerHUDWidget클래스에서 Enter or Esc키와 관련된
				//(채팅 입력창을 엔터키로 보였다가 숨겼다가 하기) 가능
				UWidgetBlueprintLibrary::SetFocusToGameViewport();
				InputChat->SetText(FText::FromString(FString("")));
		}), 0.2f, false);

		gi->SendChatMessage(TCHAR_TO_UTF8(*(Text.ToString())));
	}

}

void UPlayerHUDWidget::ShowMonsterHPBar(bool b)
{
	ESlateVisibility visibleFlag = (b) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	ContainerMonsterHP->SetVisibility(visibleFlag);
}

void UPlayerHUDWidget::SetMonsterHPRatio(float r)
{
	float fMonsterHpRatio = FMath::Clamp(r, 0.0f, 1.0f);
	PB_MonsterHp->SetPercent(fMonsterHpRatio);
}

void UPlayerHUDWidget::SetPlayerStatusInfo(PlayerVO* ptrPVO)
{
	float fCurHP = FMath::Clamp(ptrPVO->CUR_HP, 0.0f, ptrPVO->MAX_HP);
	float fCurMana = FMath::Clamp(ptrPVO->CUR_MANA, 0.0f, ptrPVO->MAX_MANA);
	txtHP->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), fCurHP)));
	txtMana->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), fCurMana)));

	PB_Hp->SetPercent(ptrPVO->CUR_HP / ptrPVO->MAX_HP);
	PB_Mana->SetPercent(ptrPVO->CUR_MANA / ptrPVO->MAX_MANA);

	txtExpPer->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), ptrPVO->CUR_EXP / ptrPVO->REQ_EXP)));
	PB_Exp->SetPercent(ptrPVO->CUR_EXP / ptrPVO->REQ_EXP);
}
