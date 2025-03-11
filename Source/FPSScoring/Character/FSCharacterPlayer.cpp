// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FSCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Actor/FSBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/FSObjectPoolSubsystem.h"
#include "Game/FSGameMode.h"

AFSCharacterPlayer::AFSCharacterPlayer()
{
	bIsShooted = false;
	// FPS 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), FName("Head")); 
	Camera->bUsePawnControlRotation = true; 

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		MappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadBulletRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ReloadBullet.IA_ReloadBullet'"));
	if (nullptr != InputActionReloadBulletRef.Object)
	{
		ReloadBulletAction = InputActionReloadBulletRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShootRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Shoot.IA_Shoot'"));
	if (nullptr != InputActionLookRef.Object)
	{
		ShootAction = InputActionShootRef.Object;
	}

	BulletCount = 30;
}

void AFSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFSCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFSCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AFSCharacterPlayer::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AFSCharacterPlayer::ShootEnd);
	EnhancedInputComponent->BindAction(ReloadBulletAction, ETriggerEvent::Triggered, this, &AFSCharacterPlayer::ForceReloadBullet);
}

void AFSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	BulletCount = 30;

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) 
	{
		Subsystem->AddMappingContext(MappingContext, 0); 
	}

}

void AFSCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void AFSCharacterPlayer::SetDead()
{
}

void AFSCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if ( PlayerController )
	{
		PlayerController->SetPause(false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void AFSCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y); 
	AddMovementInput(GetActorRightVector(), MovementVector.X); 
}

void AFSCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);   
	AddControllerPitchInput(LookAxisVector.Y); 
}

void AFSCharacterPlayer::Shoot()
{
	if (bIsShooted) return;
	bIsShooted = true;

	UFSObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<UFSObjectPoolSubsystem>();
	if ( ObjectPool )
	{
		if ( BulletCount > 0 )
		{
			FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f + FVector(0.0f, 0.0f, 50.0f);
			FRotator SpawnRotation = GetControlRotation();

			AFSBullet* Bullet = Cast<AFSBullet>(ObjectPool->GetPooledObject(AFSBullet::StaticClass(), SpawnLocation, SpawnRotation));
			Bullet->Reset();

			--BulletCount;

			if ( BulletCount == 0 )
			{
				// Reload
				int32 ReloadTime = 3;
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, 
					[this, ReloadTime]() 
					{
						ReloadBullet(ReloadTime);
					}, 
					1.0f, false);
			}

			AFSGameMode* GameMode = Cast<AFSGameMode>(GetWorld()->GetAuthGameMode());
			if ( GameMode )
			{
				GameMode->SetBulletText(BulletCount, 3);
			}
		}
	}
}

void AFSCharacterPlayer::ShootEnd()
{
	bIsShooted = false;
}

void AFSCharacterPlayer::ReloadBullet(int32 InRemainedReloadTime)
{
	if ( GetWorld() == nullptr )
	{
		BulletCount = 30;
		return;
	}

	if ( InRemainedReloadTime == 1 )
	{
		BulletCount = 30;
	}
	else
	{
		--InRemainedReloadTime;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle,
			[this, InRemainedReloadTime]()
			{
				ReloadBullet(InRemainedReloadTime);
			},
			1.0f, false);
	}

	AFSGameMode* GameMode = Cast<AFSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->SetBulletText(BulletCount, InRemainedReloadTime);
	}
}

void AFSCharacterPlayer::ForceReloadBullet()
{
	if ( BulletCount <= 0 )
	{
		return;
	}

	BulletCount = 0;
	ReloadBullet(4);
}
