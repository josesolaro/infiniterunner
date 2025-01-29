// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunningPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RunningCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ARunningPlayerController::ARunningPlayerController()
{
}

void ARunningPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(MoveLeft, ETriggerEvent::Started, this, &ARunningPlayerController::TurnLeft);
		EnhancedInputComponent->BindAction(MoveRight, ETriggerEvent::Started, this, &ARunningPlayerController::TurnRight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARunningPlayerController::TurnLeft()
{
	if(_cachedMovement == nullptr)
	{
		_cachedMovement = new FVector(0, -1, 0);
	}
	else
	{
		_cachedMovement = nullptr;
	}
	
}

void ARunningPlayerController::TurnRight()
{
	if(_cachedMovement == nullptr)
	{
		_cachedMovement = new FVector(0, 1, 0);
	}
	else
	{
		_cachedMovement = nullptr;
	}
}

void ARunningPlayerController::Tick(float DeltaSeconds)
{
	if(_cachedMovement != nullptr)
	{
		GetPawn()->AddMovementInput(*_cachedMovement);
	}
}

void ARunningPlayerController::StopTurn()
{
	_cachedMovement = nullptr;
}
