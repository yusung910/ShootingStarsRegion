// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharSelectCamSplineA.generated.h"

UCLASS()
class PFONLINEGAME_API ACharSelectCamSplineA : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACharSelectCamSplineA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "SplineController")
		TSubclassOf<class AActor> ActorToMoveClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
		class USplineComponent* Spline;

	class AActor* ActorToMove;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		float TotalPathTimeController;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		bool bSplineInLoop;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		FVector vStartLoc;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		FVector vEndLoc;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		FVector vStartDir;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		FVector vEndDir;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		class AMainGMB* gmode;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveToSplinePointAt(int32 s, int32 e);

protected:


	bool bCanMoveActor;

	float StartTime;

	float moveRate = 0.0f;
};
