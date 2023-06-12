// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

#include "MainGMB.h"

#include "Confirm.h"

void UConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	btnYes->OnClicked.AddDynamic(this, &UConfirm::ConfirmYes);
}

void UConfirm::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustReleased(EKeys::Escape))
	{
		gmode->CloseAlert();
	}
}

void UConfirm::ConfirmYes()
{
	gmode->ConfirmYes();
}
