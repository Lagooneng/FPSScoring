// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FSObjectPoolSubsystem.h"
#include "Player/FSAIController.h"

UFSObjectPoolSubsystem::UFSObjectPoolSubsystem() : Super()
{
}

void UFSObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UFSObjectPoolSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UFSObjectPoolSubsystem::InitializePool(TSubclassOf<AActor> ActorClass, int32 PoolSize)
{
    if (!ActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid ActorClass"));
        return;
    }

    if (PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("ActorClass is already Initalized"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
        return;
    }

    TArray<AActor*> ActorPool;

    for (int32 i = 0; i < PoolSize; i++)
    {
        AActor* NewActor = World->SpawnActor<AActor>(ActorClass);
        if (NewActor)
        {
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            NewActor->SetActorTickEnabled(false);

            // AI
            ActiveAIBehaviorTree(NewActor, false);

            ActorPool.Emplace(NewActor);
        }
    }

    FActorArrayWrapper Wrapper;
    Wrapper.ActorArray = ActorPool;

    PooledActorsMap.Emplace(ActorClass, Wrapper);
}

AActor* UFSObjectPoolSubsystem::GetPooledObject(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("No Actor Pool : %s"), *ActorClass->GetName());
        return nullptr;
    }

    FActorArrayWrapper& ActorPool = PooledActorsMap[ActorClass];

    for (AActor* Actor : ActorPool.ActorArray)
    {
        if (Actor && Actor->IsHidden())
        {
            Actor->SetActorHiddenInGame(false);
            Actor->SetActorEnableCollision(true);
            Actor->SetActorTickEnabled(true);
            ActiveAIBehaviorTree(Actor, true);
            Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);


            //UE_LOG(LogTemp, Log, TEXT("Get Actor : %s"), *ActorClass->GetName());

            return Actor;
        }
    }

    // No Actor in Pool -> Spawn
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //UE_LOG(LogTemp, Log, TEXT("Spawn Actor : %s"), *ActorClass->GetName());
        return World->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotation, SpawnParams);
    }

	return nullptr;
}

void UFSObjectPoolSubsystem::ReturnPooledObject(AActor* Actor)
{
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Actor"));
        return;
    }

    TSubclassOf<AActor> ActorClass = Actor->GetClass();

    if (!PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("No Pool: %s"), *ActorClass->GetName());
        return;
    }

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    // AI
    ActiveAIBehaviorTree(Actor, false);
    
    FActorArrayWrapper& ActorPool = PooledActorsMap[ActorClass];

    if (!ActorPool.ActorArray.Contains(Actor))
    {
        //UE_LOG(LogTemp, Log, TEXT("Pool Actor : %s"), *ActorClass->GetName());
        ActorPool.ActorArray.Emplace(Actor);
    }
}

void UFSObjectPoolSubsystem::ActiveAIBehaviorTree(AActor* Actor, bool IsActive)
{
    if (APawn* Pawn = Cast<APawn>(Actor))
    {
        if (Pawn->GetController() == nullptr) return;

        if (AFSAIController* AIController = Cast<AFSAIController>(Pawn->GetController()))
        {
            if ( IsActive )
            {
                AIController->OnBehaviorTree();
            }
            else
            {
                AIController->OffBehaviorTree();
            }
        }
    }
}
