// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientSocket.h"
#include "CharacterCreate.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UCharacterCreate : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCreateCharacterPawn();

	UFUNCTION()
		void DoBtnKnightClick();

	UFUNCTION()
		void DoBtnGunnerClick();

	UFUNCTION()
		void MoveToBackDp();

	UFUNCTION()
		void CreateCharacter();

	void SetSelectedBtnColor();
	
private:
	UPROPERTY()
		class ACharManagementGMB* gmode;

	UPROPERTY()
		class AMainGMB* MainGMode;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnKnight;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnGunner;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnCreate;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnBack;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UEditableText* txtCharNm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* VBtnWrapper;

	FString CLASS_SEQ;

	TMap<FString, UButton*> classBtns;


};
