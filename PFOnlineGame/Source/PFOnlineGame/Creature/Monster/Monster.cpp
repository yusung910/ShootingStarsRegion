// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Animation/AnimInstance.h"
#include "UserPlayMode.h"
#include "Ginst.h"

#include "Monster.h"

AMonster::AMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetObjectAsset(meshAsset, USkeletalMesh, "SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	GetClassAsset(anim, UAnimInstance, "AnimBlueprint'/Game/Blueprints/Anim/Monster/ABP_Monster.ABP_Monster_C'");

	GetMesh()->AnimClass = anim;

	Tags.Add(FName(TEXT("Monster")));

	//기본 공격 몽타주
	GetObjectAsset(BasicAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/Monster/Attack/Monster_Attack_Montage.Monster_Attack_Montage'");

	isAttack = false;
}

void AMonster::ToMovingAndAttack(FVector dest, FVector ori)
{
	float distToDest = UKismetMathLibrary::Vector_Distance(GetActorLocation(), dest);

	if (distToDest <= monVO.TraceRange)
	{
		float yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), dest).Yaw;

		SetActorRotation(FRotator(0, yaw, 0));
	}

	if (distToDest >= monVO.HitRange && distToDest <= monVO.TraceRange)
	{
		FVector tmp = (distToDest >= (monVO.HitRange * 2)) ? ori : dest;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), tmp);
	}

	if (distToDest <= monVO.HitRange)
	{
		Attack();
	}

}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonster::Attack()
{
	Super::Attack();

	if (!isAttack)
	{
		isAttack = true;
		PlayAnimMontage(BasicAttackMontage, 1.5, "a1_1");
	}
}

void AMonster::SendDamage()
{
	ActCollTrace(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 100.f), true, monVO.HitRange, monVO.Damage);
}

void AMonster::Dead()
{
	Super::Dead();
	K2_DestroyActor();
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	monVO.BeDamageAmount = DamageAmount;
	//피격 패킷 전송
	gi->HitMonster(monVO);

	return DamageAmount;
}
