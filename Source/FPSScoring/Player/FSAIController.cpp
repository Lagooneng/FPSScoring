// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FSAIController.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

const FName AFSAIController::PlayerKey(TEXT("Player"));

AFSAIController::AFSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjRef(TEXT("/Script/AIModule.BlackboardData'/Game/BhaviorTree/BB_FSCharacterPatrol.BB_FSCharacterPatrol'"));
	if (BBObjRef.Succeeded())
	{
		BBAsset = BBObjRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObjRef(TEXT("/Script/AIModule.BehaviorTree'/Game/BhaviorTree/BT_FSCharacterPatrol.BT_FSCharacterPatrol'"));
	if (BTObjRef.Succeeded())
	{
		BTAsset = BTObjRef.Object;
	}


}

void AFSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UBlackboardComponent* BBComponent = Blackboard;

	if (UseBlackboard(BBAsset, BBComponent))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Log, TEXT("RunBehaviorTree Failed"));
		}
		// Spawn 시 이게 작동
		AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerActor)
		{
			if (Blackboard)
			{
				Blackboard->SetValueAsObject(PlayerKey, PlayerActor);
			}
		}
	}
}

void AFSAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AFSAIController::BeginPlay()
{
	Super::BeginPlay();

	// 맵에다 Patrol 깔아두면 이게 작동
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerActor)
	{
		if ( Blackboard )
		{
			Blackboard->SetValueAsObject(PlayerKey, PlayerActor);
		}
		
		//UE_LOG(LogTemp, Log, TEXT("Blackboard Player set: %s"), *PlayerActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Player"));
	}
}

void AFSAIController::SetPlayer()
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerActor)
	{
		if (Blackboard)
		{
			Blackboard->SetValueAsObject(PlayerKey, PlayerActor);
		}
	}
}
