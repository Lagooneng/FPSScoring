// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/FSSaveGame.h"
#include "Subsystem/FSScoreSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

UFSGameInstance::UFSGameInstance()
{
	MaxScore = 0;
}

void UFSGameInstance::SaveGame()
{
	UFSSaveGame* SaveGameInstance = Cast<UFSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFSSaveGame::StaticClass()));

	if ( SaveGameInstance )
	{
		UFSScoreSubsystem* ScoreSubsystem = GetWorld()->GetSubsystem<UFSScoreSubsystem>();
		if ( ScoreSubsystem )
		{
			if ( MaxScore < ScoreSubsystem->GetScore())
			{
				SaveGameInstance->Score = ScoreSubsystem->GetScore();

				UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
			}
		}
	}
}

void UFSGameInstance::LoadGame()
{
	if ( UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))
	{
		UFSSaveGame* LoadGameInstance = Cast<UFSSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

		if ( LoadGameInstance )
		{
			MaxScore = LoadGameInstance->Score;
			UE_LOG(LogTemp, Log, TEXT("LoadGame - Score : %d"), MaxScore);
		}
	}
}

