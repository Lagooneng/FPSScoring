// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSPatrolSpawner.h"
#include "Engine/World.h"
#include "Character/FSCharacterPatrol.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFSPatrolSpawner::AFSPatrolSpawner()
{
    SpawnNum = 4;
    MaxSpawnNum = 7;

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
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FVector SpawnLocation = GetActorLocation(); 
    FRotator SpawnRotation = FRotator::ZeroRotator;

    for (int32 i = 0; i < SpawnNum; ++i)
    {
        GetWorld()->SpawnActor<AFSCharacterPatrol>(AFSCharacterPatrol::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    }

    ++SpawnNum;
    if (SpawnNum > MaxSpawnNum) SpawnNum = MaxSpawnNum;
}

// Called when the game starts or when spawned
void AFSPatrolSpawner::BeginPlay()
{
	Super::BeginPlay();
	
    SpawnPatrol();
    GetWorld()->GetTimerManager().SetTimer(SpawningHandle, this, &AFSPatrolSpawner::SpawnPatrol, 5.0f, true);
}


