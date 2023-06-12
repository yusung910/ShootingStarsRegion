// Fill out your copyright notice in the Description page of Project Settings.

#include "PFOnlineGame.h"

#include "Cmn.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "CmnAnimInst.h"

UCmnAnimInst::UCmnAnimInst()
{
	Speed = 0.f;
	Direction = 0.f;
}

void UCmnAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	IS_NULL(TryGetPawnOwner());
	Creature = Cast<ACmn>(TryGetPawnOwner());
}

void UCmnAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(Creature)) return;
	//높이(Z)를 뺀 벡터의 길이 Size2D
	Speed = Creature->GetVelocity().Size2D();
	Direction = CalculateDirection(Creature->GetVelocity(), Creature->GetActorRotation());
}
