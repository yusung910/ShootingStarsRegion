// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Cmn.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CmnAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UCmnAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCmnAnimInst();
	//블루프린트 애니메이션 초기화 함수
	virtual void NativeInitializeAnimation() override;

	//블루프린트 애니메이션 틱 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		class ACmn* Creature;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		ECondition CurrentCond; //현재 상태
};
