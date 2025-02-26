// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FSAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSSCORING_API AFSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFSAIController();

	void SetPlayer();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	static const FName PlayerKey;

private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
};
