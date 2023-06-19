// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Animation/AnimInstance.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Ginst.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Sword.h"

#include "UserPlayMode.h"

#include "User.h"

AUser::AUser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetObjectAsset(meshAsset, USkeletalMesh, "SkeletalMesh'/Game/LordVictor/Mesh/LordVictor_Elite.LordVictor_Elite'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	//캐릭터가 컨트롤러의 회전에 영향을 받지 않음
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//캐릭터가 이동 방향에 따라 회전하도록 함.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//회전시 회전 속도를 초당 640의 값으로 회전 시켜줌.
	GetCharacterMovement()->RotationRate = FRotator(0, 360.f * 4.0f, 0);

	//캐릭터의 이동을 평면으로 다니지 않음.
	GetCharacterMovement()->bConstrainToPlane = false;

	//시작시 캐릭터가 평면을 벗어난 경우 평면에 붙여줌.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// * 카메라
	//카메라 생성
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	//스프링암의 회전이 캐릭터나 컨트롤러가 아닌 월드 좌표에 따라 회전
	SpringArm->SetUsingAbsoluteRotation(true);

	SpringArm->TargetArmLength = 800.f; // 스프링 암의 길이
	SpringArm->SetRelativeRotation(FRotator(-60.f, 45.f, 0.f)); //SpringArm의 회전
	SpringArm->bDoCollisionTest = false; //충돌시 카메라 위치가 당겨지지 않도록(충돌 비활성화)
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; //pawn의 회전의 영향을 받지 않음.


	GetClassAsset(anim, UAnimInstance, "AnimBlueprint'/Game/Blueprints/Anim/User/ABP_User.ABP_User_C'");

	GetMesh()->AnimClass = anim;

	//기본 공격 몽타주
	GetObjectAsset(BasicAttackMontage, UAnimMontage, "AnimMontage'/Game/Blueprints/Montage/User/Attack/Basic_Attack.Basic_Attack'");

}

void AUser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUser::Destroyed()
{
	Super::Destroyed();

	if (sword != nullptr)
		sword->Destroy();

}

void AUser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Zoom 액션 매핑과 Zoom()함수 연결
	PlayerInputComponent->BindAxis("Zoom", this, &AUser::Zoom);
}

void AUser::RecoveryHealth(float hpAmount, float manaAmount)
{
	if (hpAmount <= 0.0f && manaAmount <= 0.0f)
	{
		pVO.CUR_HP = pVO.MAX_HP / 3;
	}

	if (hpAmount > 0.0f)
	{
		pVO.CUR_HP += hpAmount;
	}

	if (hpAmount > 0.0f)
	{
		pVO.CUR_MANA += manaAmount;
	}

}

void AUser::BeginPlay()
{
	Super::BeginPlay();

	sword = GetWorld()->SpawnActor<ASword>(ASword::StaticClass());
	sword->K2_AttachToComponent(GetMesh(), "WeaponSocket", EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	sword->SetActorEnableCollision(false);
	//Actor의 Owner를 설정
	sword->SetOwner(this);

}

float AUser::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float ret = 0.0f;

	if (DamageCauser->ActorHasTag("Monster"))
	{
		pVO.BE_DAMAGE_AMOUNT = DamageAmount;
		gi->HitPlayer(pVO);
		pVO.CUR_HP -= DamageAmount;
		ret = DamageAmount;
	}

	if (pVO.CUR_HP <= 0.0f)
	{


	}

	return ret;
}

void AUser::Attack()
{
	Super::Attack();
	PlayAnimMontage(BasicAttackMontage, 1.5);
}

void AUser::SendDamage()
{
	ActCollTrace(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 100.f), true, pVO.SINGLE_TRACE_RAD, pVO.DAMAGE);
}



void AUser::Zoom(float value)
{
	// FMath::Clamp(float x, float min, float max)
	// x값이, 두번째 인자값을 최소값, 세번째 값을 최대값 사이로 제한한다.
	// min <= x <= max
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - (value * 50.f), 100.f, 2000.f);
}
