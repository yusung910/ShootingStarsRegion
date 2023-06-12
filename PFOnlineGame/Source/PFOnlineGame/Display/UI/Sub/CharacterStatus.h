// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClientSocket.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatus.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UCharacterStatus : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void CloseStatusWidget();

	UFUNCTION()
		void IncreaseSTR();
	UFUNCTION()
		void IncreaseDEX();
	UFUNCTION()
		void IncreaseVITAL();
	UFUNCTION()
		void IncreaseENERGY();

	void SetCharacterStatus();
private: //기타 버튼

	//닫기 버튼
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnPopupClose;

private://캐릭터 정보

	//캐릭명
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtCharNm;

	//클래스 명
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtClassNm;

	//레벨
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtLevel;

	//현재 경험치
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtCurExp;

	//다음 레벨 업 필요 경험치
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtReqExp;
	
	//경험치 게이지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PB_ExpRatio;


private: //캐릭터 스탯
	//힘
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtStr;

	//민첩
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtDex;

	//체력
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtVital;

	//에너지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtEnergy;

	//잔여 스탯
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtRemainingStat;

private: //캐릭터 실제 적용 수치

	//공격력
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtDamage;

	//회피율
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtAgilityRate;

	//크리티컬 확률
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtAccuracyRate;

	//최대 체력
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtMaxHp;

	//최대 마나
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtMaxMana;

private: //스탯 상승 버튼

	//힘 상승
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnStrUp;

	//민첩 상승
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnDexUp;

	//생명력 상승
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnVitalUp;

	//에너지 버튼1
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnEnergyUp;

};
