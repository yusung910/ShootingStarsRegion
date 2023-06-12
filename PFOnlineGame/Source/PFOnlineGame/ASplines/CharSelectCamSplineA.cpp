// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Components/SplineComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainGMB.h"

#include "CharSelectCamSplineA.h"

// Sets default values
ACharSelectCamSplineA::ACharSelectCamSplineA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);
	Spline->bDrawDebug = true;

	bCanMoveActor = false;

}

// Called when the game starts or when spawned
void ACharSelectCamSplineA::BeginPlay()
{
	Super::BeginPlay();
	IS_NULL(ActorToMoveClass);

	ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform());

	gmode = Cast<AMainGMB>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ACharSelectCamSplineA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IS_NULL(ActorToMoveClass);
	// Update target
	if (bCanMoveActor)
	{
		moveRate += DeltaTime;
		gmode->SetIsCameraMoveDone(false);

		ActorToMove->SetActorLocation(UKismetMathLibrary::VLerp(vStartLoc, vEndLoc, moveRate));

		FVector Direction = UKismetMathLibrary::VLerp(vStartDir, vEndDir, moveRate);
		FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();

		ActorToMove->SetActorRotation(Rotator);

		if (moveRate >= 1.0f)
		{
			moveRate = 0.0f;
			bCanMoveActor = false;
			if (gmode->deleCameraMoveDone.IsBound())
			{
				gmode->deleCameraMoveDone.Broadcast();
			}
		}
	}
}

void ACharSelectCamSplineA::MoveToSplinePointAt(int32 s, int32 e)
{
	int32 LocalSplinePointCnt = Spline->GetNumberOfSplinePoints();
	bool ret = false;
	if (e > 0 && e < LocalSplinePointCnt)
	{
		vStartLoc = Spline->GetLocationAtSplinePoint(s, ESplineCoordinateSpace::World);
		vEndLoc = Spline->GetLocationAtSplinePoint(e, ESplineCoordinateSpace::World);

		vStartDir = Spline->GetDirectionAtSplinePoint(s, ESplineCoordinateSpace::World);
		vEndDir = Spline->GetDirectionAtSplinePoint(e, ESplineCoordinateSpace::World);

		bCanMoveActor = true;
	}
}
