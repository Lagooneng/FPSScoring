// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FSGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Game/FSGameInstance.h"
#include "Subsystem/FSObjectPoolSubsystem.h"
#include "Actor/FSBullet.h"
#include "Character/FSCharacterPatrol.h"

AFSGameMode::AFSGameMode()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetRef(TEXT("/Game/Blueprint/UI/ScoreUI.ScoreUI_C"));
    if ( WidgetRef.Succeeded() )
    {
        ScoreWidgetClass = WidgetRef.Class;
    }

    BaseString = TEXT("Prev Max Score : ");

    static ConstructorHelpers::FClassFinder<UUserWidget> MaxScoreWidgetRef(TEXT("/Game/Blueprint/UI/MaxScoreUI.MaxScoreUI_C"));
    if (MaxScoreWidgetRef.Succeeded())
    {
        MaxScoreWidgetClass = MaxScoreWidgetRef.Class;
    }
}

void AFSGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameTime = 120.0f;

    if ( ScoreWidgetClass )
    {
        ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
        ScoreWidget->AddToViewport(1);
        SetScoreText(FString::Printf(TEXT("%d"), 0));
    }

    if ( MaxScoreWidgetClass )
    {
        MaxScoreWidget = CreateWidget<UUserWidget>(GetWorld(), MaxScoreWidgetClass);
        MaxScoreWidget->AddToViewport();
        MaxScoreWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    UFSGameInstance* GameInstance = Cast<UFSGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->LoadGame();
        SetMaxScoreText(FString::Printf(TEXT("%d"), GameInstance->GetMaxScore()));
    }

    UFSObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<UFSObjectPoolSubsystem>();
    if (ObjectPool)
    {
        ObjectPool->InitializePool(AFSBullet::StaticClass(), 20);
        ObjectPool->InitializePool(AFSCharacterPatrol::StaticClass(), 60);
    }

}

void AFSGameMode::StartPlay()
{
    Super::StartPlay();
    OnPoolInitialized.Broadcast();
}

void AFSGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if ( GameTime <= 0.0f )
    {
        EndGame();
    }

    SetTimeText(GameTime);

    GameTime -= DeltaSeconds;
    if ( GameTime < 0.0f )
    {
        GameTime = 0.0f;
    }
}

void AFSGameMode::SetScoreText(FString InText)
{
	if ( ScoreWidget )
	{
        UUserWidget* WidgetInstance = ScoreWidget.Get();
        if (WidgetInstance)
        {
            UTextBlock* ScoreText = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("ScoreText")));
            if (ScoreText)
            {
                ScoreText->SetText(FText::FromString(InText));
            }
        }
	}
}

void AFSGameMode::SetTimeText(float InTime)
{
    if (ScoreWidget)
    {
        UUserWidget* WidgetInstance = ScoreWidget.Get();
        if (WidgetInstance)
        {
            UTextBlock* ScoreText = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Time")));
            if (ScoreText)
            {
                ScoreText->SetText(FText::FromString( FString::Printf(TEXT("Remained Time : %0.2f"), InTime)));
            }
        }
    }
}

void AFSGameMode::SetMaxScoreText(FString InText)
{
    if (MaxScoreWidget)
    {
        UUserWidget* WidgetInstance = MaxScoreWidget.Get();
        if (WidgetInstance)
        {
            UTextBlock* ScoreText = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("ScoreText")));
            if (ScoreText)
            {
                FString Result = BaseString + InText;
                ScoreText->SetText(FText::FromString(Result));
            }
        }
    }
}

void AFSGameMode::EndGame()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetPause(true);
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeGameAndUI());
    }

    UFSGameInstance* GameInstance = Cast<UFSGameInstance>(GetWorld()->GetGameInstance()); 
    if (GameInstance) 
    {
        GameInstance->SaveGame(); 

    }
    
    MaxScoreWidget->SetVisibility(ESlateVisibility::Visible);
}
