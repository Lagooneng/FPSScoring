// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FSBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/FSCollision.h"
#include "Subsystem/FSScoreSubsystem.h"
#include "Game/FSGameMode.h"
#include "Subsystem/FSObjectPoolSubsystem.h"
#include "Character/FSCharacterPatrol.h"

// Sets default values
AFSBullet::AFSBullet()
{
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(5.0f);
	Trigger->SetCollisionProfileName(CPROFILE_FSTRIGGER);
	RootComponent = Trigger;
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFSBullet::OnComponentBeginOverlapCallback);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/SM_Bullet.SM_Bullet'"));
	
	if (StaticMeshRef.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(StaticMeshRef.Object);
	}

	StaticMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1, 0.1f));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.0f; 
	Movement->MaxSpeed = 3000.0f;     
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;

	//InitialLifeSpan = 3.0f;
}

void AFSBullet::Reset()
{
	if (Movement)
	{
		Movement->StopMovementImmediately();
		Movement->Velocity = GetActorForwardVector() * Movement->InitialSpeed;
	}

	GetWorld()->GetTimerManager().SetTimer(PoolingTimer, this, &AFSBullet::PoolBullet, 3.0f, false);
}

void AFSBullet::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UFSScoreSubsystem* ScoreSubsystem = GetWorld()->GetSubsystem<UFSScoreSubsystem>();
	if (ScoreSubsystem)
	{
		ScoreSubsystem->SetScore(ScoreSubsystem->GetScore() + 1);
		UE_LOG(LogTemp, Log, TEXT("Hit! - %d"), ScoreSubsystem->GetScore());

		AFSGameMode* GameMode = Cast<AFSGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->SetScoreText(FString::FromInt(ScoreSubsystem->GetScore()));
	}

	if ( Cast<AFSCharacterPatrol>(OtherActor) )
	{
		UFSObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<UFSObjectPoolSubsystem>();
		if (ObjectPool)
		{
			ObjectPool->ReturnPooledObject(OtherActor);
		}
	}

	PoolBullet();
}

void AFSBullet::PoolBullet()
{
	UFSObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<UFSObjectPoolSubsystem>();
	if (ObjectPool)
	{
		ObjectPool->ReturnPooledObject(Cast<AActor>(this));
	}
}
