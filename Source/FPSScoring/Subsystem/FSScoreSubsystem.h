// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FSScoreSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FPSSCORING_API UFSScoreSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFSScoreSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FORCEINLINE void SetScore(int32 InScore) { Score = InScore; }
	FORCEINLINE const int32 GetScore() const { return Score; }
private:
	int32 Score;
};
