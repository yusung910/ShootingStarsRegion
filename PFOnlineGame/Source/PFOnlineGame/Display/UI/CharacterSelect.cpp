// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Button.h"

#include "CharacterRow.h"

#include "CharManagementGMB.h"
#include "MainGMB.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "CharacterSelect.h"


UCharacterSelect::UCharacterSelect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetClassAsset(CharEntryAsset, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/Sub/BPW_CharRow.BPW_CharRow_C'");

}

void UCharacterSelect::NativeConstruct()
{

}

void UCharacterSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//게임모드 세팅
	gmode = Cast<ACharManagementGMB>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGMode = Cast<AMainGMB>(gmode);

	btnCreate->OnClicked.AddDynamic(this, &UCharacterSelect::ShowCharacterGenerateDisplay);
	btnExit->OnClicked.AddDynamic(this, &UCharacterSelect::DoGameEnd);
}

void UCharacterSelect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterSelect::InitCharacterList(vector<PlayerVO> chars)
{
	CharListContainer->ClearChildren();
	VBCharacterListWrapper->ClearChildren();

	for (auto charInfo : chars)
	{
		UUserWidget* CharEntryWidget = CreateWidget<UUserWidget>(GetWorld(), CharEntryAsset);
		VBCharacterListWrapper->AddChild(CharEntryWidget);
		Cast<UCharacterRow>(CharEntryWidget)->SetCharinfo(charInfo);

		charEntryWidgetList.Add(CharEntryWidget);
	}

	CharListContainer->AddChild(VBCharacterListWrapper);
}

void UCharacterSelect::InitCharSelectBtnColor()
{
	for (auto entry : charEntryWidgetList)
	{
		Cast<UCharacterRow>(entry)->InitSelectBtnColor();
	}
}

void UCharacterSelect::ShowCharacterGenerateDisplay()
{
	//화면 이동
	gmode->ShowCreateCharacterWidget();
}

void UCharacterSelect::DoGameEnd()
{
	MainGMode->GameEnd();
}
