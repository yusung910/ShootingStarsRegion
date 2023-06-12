// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/Cmn.h"
#include "MonsterVO.h"
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AMonster : public ACmn
{
	GENERATED_BODY()
public:
	AMonster();
	void SetMonsterVO(MonsterVO* _monVO);
	void MoveToLocation(FVector dest);

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void SendDamage() override;

	virtual void Dead() override;

	FORCEINLINE int GetMonsterId() { return monVO.Id; };
	FORCEINLINE void SetMonsterCond(ECondition cond) { monVO.MonsterCond = cond; }

	FORCEINLINE void SetAttackTargetLoc(FVector v) { AttackTargetLoc = v; }

	FORCEINLINE float GetMonsterHPRatio() { return monVO.CUR_HP / monVO.MAX_HP; }

protected:
	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* BasicAttackMontage;
private:
	MonsterVO monVO;
	//
	FVector AttackTargetLoc;
	
	bool bDeadDelayFlag;
	float fDeadDelayCount;
};
