// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Display/Message/Alert.h"
#include "Confirm.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UConfirm : public UAlert
{
	GENERATED_BODY()
public:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
		void ConfirmYes();

	UPROPERTY(meta = (BindWidget))
		class UButton* btnYes;
};
