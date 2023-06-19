// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumCreatures.h"
#include "PlayerVO.h"
#include "MonsterVO.h"

#include "Cmn.generated.h"




UCLASS()
class PFONLINEGAME_API ACmn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACmn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//ApplyDamage함수로 데미지를 전달받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	//EnemyActor들이 플레이어에게 데미지 주기 위한 함수.
	void ActCollTrace(FVector s, FVector e, bool isSingle, float rad, float dmg);

	UPROPERTY()
		float DestroyTimeAccumulate = 0.0f;

	UPROPERTY()
		float DestroyLimitTime = 3.0f;

	UPROPERTY()
		class USkeletalMesh* meshAsset;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* HitZone;

	UPROPERTY()
		TSubclassOf<UAnimInstance> anim;

	UPROPERTY()
		class AUserPlayMode* upGameMode;

	//다른 캐릭터들(몬스터, 유저 등)에게 가할 데미지
	float DAMAGE;

	//공격 실행
	virtual void Attack() {};

	//데미지 전달
	virtual void SendDamage() {};

	//캐릭터 사망
	virtual void Dead() {};

	//게임 인스턴스
	UPROPERTY()
		class UGinst* gi;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
