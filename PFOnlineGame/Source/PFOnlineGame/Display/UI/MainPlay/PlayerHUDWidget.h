// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Containers/Queue.h"
#include "PlayerVO.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowMonsterHPBar(bool b);

	void SetMonsterHPRatio(float r);

	void SetPlayerStatusInfo(PlayerVO* ptrPVO);

	void SetVisibleChatInput();

	void SetChatMsgAtWindow(const string* msg);

	UFUNCTION()
		void SendChatMsg(const FText& Text, ETextCommit::Type CommitMethod);
private:

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* SB_ChatLog;

	UPROPERTY(meta = (BindWidget))
		class UEditableText* InputChat;

	UPROPERTY(meta = (BindWidget))
		class UBorder* ChatInputContainer;

	//채팅메세지
	TQueue<UTextBlock*> chatQueue;

	FScriptDelegate deleChatInput;
private:
	//몬스터 체력, 이름
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UOverlay* ContainerMonsterHP;
	//몬스터 체력
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PB_MonsterHp;
	//몬스터 이름
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtMonsterNm;

private: //사용자 체력, 경험치, 마나 게이지 및 텍스트
	//체력 게이지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PB_Hp;

	//체력 수치
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtHP;

	//마나 게이지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PB_Mana;

	//마나 수치
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtMana;

	//경험치 게이지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PB_Exp;

	//경험치 퍼센트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtExpPer;


private: //사용자 스킬 사용 버튼 및 텍스트
	//스킬 버튼1
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSkill1;

	//스킬 버튼1 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSkillKey1;

	//스킬 버튼2
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSkill2;

	//스킬 버튼2 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSkillKey2;

	//스킬 버튼3
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSkill3;

	//스킬 버튼3 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSkillKey3;

	//스킬 버튼4
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSkill4;

	//스킬 버튼4 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSkillKey4;

	//스킬 버튼5
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnSkill5;

	//스킬 버튼5 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtSkillKey5;
	

private: //사용자 소모품 사용 버튼 및 텍스트
	//스킬 버튼1
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnItem1;

	//스킬 버튼1 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtItemKey1;

	//스킬 버튼2
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnItem2;

	//스킬 버튼2 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtItemKey2;

	//스킬 버튼3
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnItem3;

	//스킬 버튼3 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtItemKey3;

	//스킬 버튼4
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnItem4;

	//스킬 버튼4 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtItemKey4;

	//스킬 버튼5
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* btnItem5;

	//스킬 버튼5 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* txtItemKey5;

private:
	UPROPERTY()
		class UGinst* gi;
};
