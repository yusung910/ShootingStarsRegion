// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class PFONLINEGAME_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY()
		class UStaticMeshComponent* MyMesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
