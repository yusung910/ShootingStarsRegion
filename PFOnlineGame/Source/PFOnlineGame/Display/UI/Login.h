// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Login.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API ULogin : public UUserWidget
{
	GENERATED_BODY()
	
		
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void DoLogin();

	UFUNCTION()
		void DoGameExit();

	UFUNCTION()
		void PwCommitted(const FText& Text, ETextCommit::Type CommitMethod);


	FScriptDelegate delPwInput;
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* btnLogin;

	UPROPERTY(meta = (BindWidget))
		class UButton* btnExit;
	
	UPROPERTY(meta = (BindWidget))
		class UEditableText* UserPW;

	UPROPERTY(meta = (BindWidget))
		class UEditableText* UserID;

	UPROPERTY()
		class ACharManagementGMB* gmode;

};