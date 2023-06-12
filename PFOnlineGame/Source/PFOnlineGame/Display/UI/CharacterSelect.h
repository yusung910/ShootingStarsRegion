// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientSocket.h"

#include "CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UCharacterSelect : public UUserWidget
{
	GENERATED_BODY()
public:
	UCharacterSelect(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitCharacterList(vector<PlayerVO> chars);

	void InitCharSelectBtnColor();

	UFUNCTION()
		void ShowCharacterGenerateDisplay();

	UFUNCTION()
		void DoGameEnd();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UScrollBox* CharListContainer;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* VBCharacterListWrapper;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnCreate;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnExit;


	UPROPERTY()
		TSubclassOf<class UUserWidget> CharEntryAsset; //메인 화면 위젯

	UPROPERTY()
		class ACharManagementGMB* gmode;

	UPROPERTY()
		class AMainGMB* MainGMode;

	TArray<class UUserWidget*> charEntryWidgetList;
	
	
};
