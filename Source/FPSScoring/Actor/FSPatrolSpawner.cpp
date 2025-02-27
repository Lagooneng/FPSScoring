// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSPatrolSpawner.h"
#include "Engine/World.h"
#include "Character/FSCharacterPatrol.h"
#include "Components/StaticMeshComponent.h"
#include "Subsystem/FSObjectPoolSubsystem.h"
#include "Game/FSGameMode.h"

// Sets default values
AFSPatrolSpawner::AFSPatrolSpawner()
{
    SpawnCount = 0;
    MaxSpawnCount = 30;

    Offset = 500.0f;

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    RootComponent = StaticMeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/MaterialSphere.MaterialSphere'"));
    if ( StaticMeshRef.Succeeded() )
    {
        StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
    }
}

void AFSPatrolSpawner::SpawnPatrol()
{
    if (SpawnCount >= MaxSpawnCount) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FVector SpawnLocation = GetActorLocation(); 
    SpawnLocation.X += FMath::RandRange(-Offset, Offset);
    SpawnLocation.Y += FMath::RandRange(-Offset, Offset);

    FRotator SpawnRotation = FRotator::ZeroRotator;

    UFSObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<UFSObjectPoolSubsystem>();
    if (ObjectPool)
    {
        if (SpawnCount >= MaxSpawnCount) return;

        ++SpawnCount;
        ObjectPool->GetPooledObject(AFSCharacterPatrol::StaticClass(), SpawnLocation, SpawnRotation);
    }
}

void AFSPatrolSpawner::BeginPlay()
{
	Super::BeginPlay();
	
    if ( AFSGameMode* GameMode = Cast<AFSGameMode>(GetWorld()->GetAuthGameMode()) )
    {
        GameMode->OnPoolInitialized.AddDynamic(this, &AFSPatrolSpawner::StartSpawn);
    }
}

void AFSPatrolSpawner::StartSpawn()
{
    SpawnPatrol();
    GetWorld()->GetTimerManager().SetTimer(SpawningHandle, this, &AFSPatrolSpawner::SpawnPatrol, 1.0f, true);
}


