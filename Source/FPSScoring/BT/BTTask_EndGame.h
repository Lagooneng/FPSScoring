// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EndGame.generated.h"

/**
 * 
 */
UCLASS()
class FPSSCORING_API UBTTask_EndGame : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EndGame();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
