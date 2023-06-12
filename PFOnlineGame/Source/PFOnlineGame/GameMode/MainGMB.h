// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SktCorresCmn.h"
#include "ClientSocket.h"

#include "MainGMB.generated.h"

DECLARE_MULTICAST_DELEGATE(FDeleAlertBtn)
DECLARE_MULTICAST_DELEGATE(FDeleConfirmBtn)
DECLARE_MULTICAST_DELEGATE(FDeleCameraMoveDone)

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AMainGMB : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGMB();


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Message")
		void ShowAlert(FString msg);

	UFUNCTION(BlueprintCallable, Category = "Message")
		void SetMessageCopy(FString msg);

	UFUNCTION(BlueprintCallable, Category = "Message")
		void SetVisibleBtnConfirm(bool b);

	UFUNCTION(BlueprintCallable, Category = "Message")
		void ShowConfirm(FString msg);

	UFUNCTION(BlueprintCallable, Category = "Message")
		void CloseAlert();

	UFUNCTION(BlueprintCallable, Category = "Message")
		void ConfirmYes();

	UFUNCTION()
		void GameEnd();

	UFUNCTION()
		void ShowErrorMsg(EServerStatusCode code);

	void StartMainGamePlay(PlayerVO vo);

	bool CheckUpServerCorrespondenceStatus(EServerStatusCode code);
	

	FDeleAlertBtn deleAlertBtn;
	FDeleConfirmBtn deleConfirmBtn;
	FDeleCameraMoveDone deleCameraMoveDone;

	FORCEINLINE void SetIsCameraMoveDone(bool b) { IsCameraMoveDone = b; }

protected:


	UPROPERTY()
		TSubclassOf<class UUserWidget> AlertWidgetClass; //얼럿 팝업 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> ConfirmWidgetClass; //컨펌 팝업 위젯


	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY()
		class UUserWidget* AlertWidget;

	UPROPERTY()
		class UUserWidget* ConfirmWidget;



protected: //network


	bool bIsConnected;

	//TArray<TMap<FString, FString>> Chars;


	bool IsCameraMoveDone = false;


	UPROPERTY()
		class UGinst* gi;


private:


};
