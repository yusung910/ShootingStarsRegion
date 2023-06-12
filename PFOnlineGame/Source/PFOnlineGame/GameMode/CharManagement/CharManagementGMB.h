// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/MainGMB.h"
#include "CharManagementGMB.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API ACharManagementGMB : public AMainGMB
{
	GENERATED_BODY()
	
public:

	ACharManagementGMB();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Login(const FText& Id, const FText& Pw);

	void CreateCharacter(PlayerVO vo);

	void DeleteCharacter(PlayerVO vo);
public:		//데이터 송수신
	void SetCharacters(vector<PlayerVO> chars);

	//캐릭터 선택화면 위젯을 보여준다.
	void ShowCharacterSelectWidget();

	//캐릭터 생성화면 위젯을 보여준다
	void ShowCreateCharacterWidget();

	//캐릭터 생성화면에서 목록화면으로 이동한다
	void MoveCamCharSelectFromCreate();

public:
	//캐릭터 선택 버튼 컬러를 초기화한다.
	void InitCharSelectBtnColor();

	//플레이어의 캐릭터 프리뷰를 보여준다.
	void SpawnUserActorInCharacterSelect(PlayerVO vo);

	//생성할 캐릭터의 프리뷰 액터를 보여준다
	void SpawnCharActorInCreateCharacter(FString classSeq);

	//캐릭터 선택 위젯화면을 일시적으로 정지 시킨다. (true : 활성화, false : 비활성화)
	void SetCharListWidgetEnable(bool b);


	FORCEINLINE FString GetSelectedCharSeq() { return SelectedCharSeq; }
	FORCEINLINE void SetSelectedCharSeq(FString str) { SelectedCharSeq = str; }

private: //캐릭터
	UPROPERTY()
		class AUser* userActor;

	UPROPERTY()
		class AUser* CreateActor;

	UPROPERTY()
		class ACharSelectCamSplineA* CamSplineActor;


	vector<PlayerVO> UserCharList;


	FString SelectedCharSeq;

	FVector UserCharLoc;
	FVector CreateCharLoc;

	UPROPERTY()
		TSubclassOf<class UUserWidget> LoginWidgetClass; //메뉴 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> CharListWidgetClass; //컨펌 팝업 위젯

	UPROPERTY()
		TSubclassOf<class UUserWidget> CharCreateWidgetClass; //캐릭터 생성 위젯

	UPROPERTY()
		class UUserWidget* CharListWidget;

	UPROPERTY()
		class UUserWidget* CharCreateWidget;

};
