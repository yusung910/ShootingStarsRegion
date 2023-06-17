// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cmn.h"
#include "User.generated.h"

/**
 * 
 */
UCLASS()
class PFONLINEGAME_API AUser : public ACmn
{
	GENERATED_BODY()
public:
	AUser();
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// called Destroyed
	virtual void Destroyed() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack() override;

	virtual void SendDamage() override;

	FORCEINLINE PlayerVO GetPlayerVO() { return pVO; }

	FORCEINLINE void SetPlayerVO(PlayerVO vo) { pVO = vo; }

	FORCEINLINE void SetPlayerCond(ECondition cond) { pVO.PlayerCond = cond; }

	FORCEINLINE int GetPlayerSessionId() { return pVO.SessionID; }
	FORCEINLINE void SetPlayerSessionId(int n) { pVO.SessionID = n; }

	void RecoveryHealth(float hpAmount, float manaAmount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//블루프린트 AnyDamage의 C++버전
	//ApplyDamage를 통해 들어온 데미지를 전달받는다.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* BasicAttackMontage;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UFUNCTION()
		void Zoom(float value);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "weapon", meta = (AllowPrivateAccess = "true"))
		class ASword* sword;

	//플레이어 정보가 담긴 포인터 변수
	PlayerVO pVO;
};
