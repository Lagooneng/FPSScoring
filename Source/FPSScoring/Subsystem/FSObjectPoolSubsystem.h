// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FSObjectPoolSubsystem.generated.h"

USTRUCT()
struct FActorArrayWrapper
{
	GENERATED_BODY()

    UPROPERTY()
    TArray< TObjectPtr<AActor> > ActorArray;
};

/**
 * 
 */
UCLASS()
class FPSSCORING_API UFSObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFSObjectPoolSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void InitializePool(TSubclassOf<AActor> ActorClass, int32 PoolSize);

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    AActor* GetPooledObject(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation, const FRotator& SpawnRotation);

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void ReturnPooledObject(AActor* Actor);

protected:
    void ActiveAIBehaviorTree(AActor* Actor, bool IsActive);

    UPROPERTY()
    TMap<TSubclassOf<AActor>, FActorArrayWrapper > PooledActorsMap;
};
