// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FSCharacterPatrol.h"
#include "Player/FSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFSCharacterPatrol::AFSCharacterPatrol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Skeleton_Guard/Mesh_UE4/Full/SKM_Skeleton_Guard.SKM_Skeleton_Guard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	AIControllerClass = AFSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
}
