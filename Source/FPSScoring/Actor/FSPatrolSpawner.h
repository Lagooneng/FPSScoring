// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPatrolSpawner.generated.h"

UCLASS()
class FPSSCORING_API AFSPatrolSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSPatrolSpawner();
	void SpawnPatrol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	FTimerHandle SpawningHandle;

private:
	int32 SpawnNum;
	int32 MaxSpawnNum;
};
