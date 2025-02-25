// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FSScoreSubsystem.h"

UFSScoreSubsystem::UFSScoreSubsystem()
{
	Score = 0;
}

void UFSScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Score = 0;
}

void UFSScoreSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
