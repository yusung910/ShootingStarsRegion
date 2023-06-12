// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Monster.h"

#include "MonsterAnimInst.h"


UMonsterAnimInst::UMonsterAnimInst()
{

}

void UMonsterAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	IS_NULL(TryGetPawnOwner());
	monster = Cast<AMonster>(TryGetPawnOwner());
}

void UMonsterAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterAnimInst::AnimNotify_Damage()
{
	if (!IsValid(monster)) return;
	monster->SendDamage();
}

void UMonsterAnimInst::AnimNotify_EndAttack()
{
	if (!IsValid(monster)) return;
	monster->SetMonsterCond(ECondition::IS_IDLE);
}
