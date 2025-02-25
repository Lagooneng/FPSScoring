// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTask_EndGame.h"
#include "Game/FSGameInstance.h"
#include "Game/FSGameMode.h"

UBTTask_EndGame::UBTTask_EndGame()
{
	NodeName = TEXT("EndGame");
}

EBTNodeResult::Type UBTTask_EndGame::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AFSGameMode* GameMode = Cast<AFSGameMode>(GetWorld()->GetAuthGameMode()); 
	if (GameMode) 
	{
		GameMode->EndGame(); 
	}
	
	return EBTNodeResult::Succeeded;
}
