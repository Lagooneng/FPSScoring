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
	AFSPatrolSpawner();
	void SpawnPatrol();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartSpawn();

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	FTimerHandle SpawningHandle;

private:
	int32 SpawnCount;
	int32 MaxSpawnCount;

	float Offset;
};
