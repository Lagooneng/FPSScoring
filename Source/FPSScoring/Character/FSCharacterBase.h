// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FSCharacterBase.generated.h"

UCLASS()
class FPSSCORING_API AFSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFSCharacterBase();

protected:
	virtual void SetDead();
};
