// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientSocket.h"
#include "CharacterRow.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UCharacterRow : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCharinfo(PlayerVO info);

	UFUNCTION()
		void CharacterPreviewChange();

	UFUNCTION()
		void DeleteCharacterConfirm();


	void InitSelectBtnColor();
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtCharName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtCharLvl;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtCharClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnStart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnDelete;

	UPROPERTY()
		class ACharManagementGMB* gmode;

	UPROPERTY()
		class AMainGMB* MainGMode;

	PlayerVO charVO;


	bool isStartClicked = false;
	bool isDblClicked = false;
	float dblTermTimer = 0.0f;
};