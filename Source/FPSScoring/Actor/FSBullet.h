// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSBullet.generated.h"

UCLASS()
class FPSSCORING_API AFSBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AFSBullet();

protected:
	UFUNCTION()
	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UPROPERTY()
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY()
	TObjectPtr<class UProjectileMovementComponent> Movement;
};
