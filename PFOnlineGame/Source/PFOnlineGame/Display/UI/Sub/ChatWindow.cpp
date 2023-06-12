// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "ChatWindow.h"

void UChatWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

