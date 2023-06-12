// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"

#include "Ginst.h"

#include "Cmn.h"
// Sets default values
ACmn::ACmn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	HitZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitZone"));
	HitZone->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	HitZone->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	HitZone->SetCollisionProfileName(FName("PhysicsActor"));
	HitZone->SetupAttachment(GetMesh());

	HitZone->CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void ACmn::BeginPlay()
{
	Super::BeginPlay();

	//게임 인스턴스 세팅
	gi = Cast<UGinst>(GetWorld()->GetGameInstance());
}

// Called every frame
void ACmn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//LOG_SCREEN_T("Loc X, Y, Z : %f, %f, %f", GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
}

// Called to bind functionality to input
void ACmn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACmn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void ACmn::ActCollTrace(FVector s, FVector e, bool isSingle, float rad, float dmg)
{
	//충돌가능 오브젝트 유형
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);	//폰타입

	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);	//무시할 액터 배열
	
	bool bIsLocalTrace = false;
	
	if (isSingle)
	{
		FHitResult hitResult; //sinlge

		bIsLocalTrace = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), s, e, rad,
			ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, hitResult, true,
			FLinearColor::Red, FLinearColor::Green, 0.0f);

		if (bIsLocalTrace)
		{
			UGameplayStatics::ApplyDamage(hitResult.GetActor(), dmg, GetController(), this, UDamageType::StaticClass());
		}
	}
	else
	{
		TArray<FHitResult> hitResults; //multi

		bIsLocalTrace = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), s, e, rad,
			ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, hitResults, true,
			FLinearColor::Red, FLinearColor::Green, 0.0f);

		if (bIsLocalTrace)
		{
			TArray<ACharacter*> DamagedActorList; //targeting 객체 목록

			for (auto hitR : hitResults)
			{
				ACharacter* hitActor = Cast<ACharacter>(hitR.GetActor());
				if (hitActor != nullptr )
				{
					DamagedActorList.AddUnique(Cast<ACharacter>(hitR.GetActor()));
				}
			}

			for (auto actor : DamagedActorList)
			{
				UGameplayStatics::ApplyDamage(actor, dmg, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}
