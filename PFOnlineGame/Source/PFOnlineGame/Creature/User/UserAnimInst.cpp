// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "User.h"

#include "UserAnimInst.h"

UUserAnimInst::UUserAnimInst()
{

}

void UUserAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	IS_NULL(TryGetPawnOwner());
	user = Cast<AUser>(TryGetPawnOwner());
}

void UUserAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UUserAnimInst::AnimNotify_Damage()
{
	if (!IsValid(user)) return;
	user->SendDamage();
}

void UUserAnimInst::AnimNotify_EndAttack()
{
	if (!IsValid(user)) return;
	user->SetPlayerCond(ECondition::IS_IDLE);
}
