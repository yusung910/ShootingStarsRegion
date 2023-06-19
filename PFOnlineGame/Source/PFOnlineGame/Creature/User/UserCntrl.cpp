// Fill out your copyright notice in the Description page of Project Settings.

#include "PFOnlineGame.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Cmn.h"
#include "GInst.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Monster.h"
#include "UserPlayMode.h"
#include "User.h"
#include "UserPlayMode.h"
#include "UserNetworkCntrl.h"

#include "UserCntrl.h"

AUserCntrl::AUserCntrl()
{
	//마우스 커서를 보여준다.
	bShowMouseCursor = true;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUserCntrl::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AUserCntrl::SetupInputComponent()
{
	Super::SetupInputComponent();

	//마우스 눌렀을 때 함수 실행
	InputComponent->BindAction("Click", IE_Pressed, this, &AUserCntrl::GetMouseButtonDown);

	//마우스 눌렀다가 뗐을 때 함수 실행 
	InputComponent->BindAction("Click", IE_Released, this, &AUserCntrl::GetMouseButtonUp);

	//캐릭터 스탯창 호출
	InputComponent->BindAction("ShowStatus", IE_Released, this, &AUserCntrl::ShowCharacterStatus);
}

void AUserCntrl::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (isClick)
	{
		DoLeftClickAction();
	}

	ShowMonsterInfo();
}

void AUserCntrl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//몬스터 체력 잔여 퍼센트 표시
	if (aClickedMonster != nullptr)
	{
		gmode->SetMonsterHPRatio(Cast<AMonster>(aClickedMonster)->GetMonsterHPRatio());
	}
}

void AUserCntrl::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUserCntrl::BeginPlay()
{
	Super::BeginPlay();
}


void AUserCntrl::GetMouseButtonDown()
{
	if (UserCharacter->GetPlayerVO().PlayerCond == ECondition::IS_IDLE)
	{
		isClick = true;
	}
}

void AUserCntrl::GetMouseButtonUp()
{
	isClick = false;
}

void AUserCntrl::DoLeftClickAction()
{
	FHitResult hitInfo;
	GetHitResultUnderCursor(ECC_PhysicsBody, false, hitInfo);
	//GetHitResultUnderCursor : 마우스 커서 위치로 트레이스를 발사하여 결과를 HitResult에 넣어줌.
	if (hitInfo.bBlockingHit)
	{
		PlayerVO LocalPVO = UserCharacter->GetPlayerVO();
		ECondition PlayerCond = LocalPVO.PlayerCond;
		bool bClickMonsterAttack = false;

		if (hitInfo.GetActor()->ActorHasTag("Monster"))
		{
			float PlayerToMonsterDist = UKismetMathLibrary::Vector_Distance(UserCharacter->GetActorLocation(), hitInfo.GetActor()->GetActorLocation());

			bClickMonsterAttack = (PlayerToMonsterDist < LocalPVO.SINGLE_TRACE_RAD);
		}
		
		if ((IsInputKeyDown(EKeys::LeftShift) && PlayerCond != ECondition::IS_ATTACK) || (bClickMonsterAttack && PlayerCond != ECondition::IS_ATTACK))
		{
			float yaw = UKismetMathLibrary::FindLookAtRotation(UserCharacter->GetActorLocation(), hitInfo.ImpactPoint).Yaw;

			UserCharacter->SetActorRotation(FRotator(0, yaw, 0));
			//어택 몽타주 재생
			UserCharacter->Attack();
			UserCharacter->SetPlayerCond(ECondition::IS_ATTACK);
		}

		if (PlayerCond == ECondition::IS_IDLE)
		{
			//FVector::Dist : 두 지점의 거리를 계산
			float const distance = FVector::Dist(hitInfo.ImpactPoint, UserCharacter->GetActorLocation());

			//그 거리가 100보다 크다면
			if (distance > 100.f)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, hitInfo.ImpactPoint);
				//SimpleMoveToLocation : 네비 메시 바운드 볼륨 기반으로 도착지점으로 이동 시켜주는 함수.
			}
		}
		
		//if (hitInfo.GetActor()->ActorHasTag("Monster"))
		//{
		//	aClickedMonster = hitInfo.GetActor();
		//	gmode->ShowMonsterHPBar(true);

		//}
		//else
		//{
		//	gmode->ShowMonsterHPBar(false);
		//	aClickedMonster = nullptr;
		//}
	}

}

void AUserCntrl::ShowMonsterInfo()
{
	FHitResult hitInfo;
	GetHitResultUnderCursor(ECC_PhysicsBody, false, hitInfo);
	//GetHitResultUnderCursor : 마우스 커서 위치로 트레이스를 발사하여 결과를 HitResult에 넣어줌.
	if (hitInfo.bBlockingHit)
	{
		if (hitInfo.GetActor()->ActorHasTag("Monster"))
		{
			aClickedMonster = hitInfo.GetActor();
			gmode->ShowMonsterHPBar(true);
		}
		else
		{
			gmode->ShowMonsterHPBar(false);
			aClickedMonster = nullptr;
		}
	}
}

//캐릭터 스텟창 호출
void AUserCntrl::ShowCharacterStatus()
{
	gmode->ShowPlayerStatusWidget();
}
