// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>
using namespace std;


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Alert.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UAlert : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TxtContent;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnConfirm;

	//UFUNCTION()
	void SetMessageCopy(FString message);

	//버튼을 보여줄것인지 세팅한다.
	void SetVisibleBtnConfirm(bool bFlag);

	UFUNCTION()
		void CloseAlert();

protected:
	UPROPERTY()
		class AMainGMB* gmode;

	float focusTimer = 0.0f;

	bool isSetFocus = false;
};
