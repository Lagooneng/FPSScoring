// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSSCORING_API UFSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFSGameInstance();

	void SaveGame();
	void LoadGame();
	FORCEINLINE int32 GetMaxScore() { return MaxScore; }

private:
	int32 MaxScore;
};
