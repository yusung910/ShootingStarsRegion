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
	
	void ToMovingAndAttack(FVector dest, FVector ori);

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void SendDamage() override;

	virtual void Dead() override;

	FORCEINLINE int GetMonsterId() { return monVO.Id; };

	FORCEINLINE float GetMonsterHPRatio() { return CurHPRatio; }
	FORCEINLINE void SetMonsterHPRatio(float _f) { CurHPRatio = _f; }

	FORCEINLINE void SetIsAttack(bool b) { isAttack = b; }
	FORCEINLINE bool GetIsAttack() { return isAttack; }

	//FORCEINLINE void SetMonsterCurHP(float f) { monVO.CUR_HP = f; }
	FORCEINLINE void SetMonsterVO(MonsterVO _monVO) { monVO = _monVO; };
protected:
	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* BasicAttackMontage;

private:
	MonsterVO monVO;

	bool isAttack;
	
	float CurHPRatio;
};
