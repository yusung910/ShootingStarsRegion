// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/CmnAnimInst.h"
#include "UserAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API UUserAnimInst : public UCmnAnimInst
{
	GENERATED_BODY()
public:
	UUserAnimInst();

	//블루프린트 애니메이션 초기화 함수
	virtual void NativeInitializeAnimation() override;

	//블루프린트 애니메이션 틱 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		class AUser* user;

protected:
	UFUNCTION()
		void AnimNotify_Damage();
	UFUNCTION()
		void AnimNotify_EndAttack();
};
