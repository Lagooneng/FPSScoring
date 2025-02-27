// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPoolInitialized);

/**
 * 
 */
UCLASS()
class FPSSCORING_API AFSGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AFSGameMode();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetScoreText(FString InText);
	void SetTimeText(float InTime);
	void SetMaxScoreText(FString InText);
	void EndGame();

	FOnPoolInitialized OnPoolInitialized;

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UUserWidget> ScoreWidget;

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> MaxScoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UUserWidget> MaxScoreWidget;

private:
	FString BaseString;
	float GameTime;
};
