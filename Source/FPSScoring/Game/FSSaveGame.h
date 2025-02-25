// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FPSSCORING_API UFSSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFSSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "SaveGame Data")
	int32 Score;
};
