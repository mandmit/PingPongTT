// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"
#include "Pawn/PlayerPlatform.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "UI/MainHUD.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
		EnhancedInputComponent->BindAction(TiltAction, ETriggerEvent::Started, this, &ABasePlayerController::Tilt);
		EnhancedInputComponent->BindAction(StartMatchAction, ETriggerEvent::Started, this, &ABasePlayerController::Approve);
	}
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();
	if (APlayerPlatform* Platform = GetPawn<APlayerPlatform>())
	{
		Platform->Move(Movement);
	}
}

void ABasePlayerController::Tilt(const FInputActionValue& Value)
{
	float AngleValue = Value.Get<float>();
	if (APlayerPlatform* Platform = GetPawn<APlayerPlatform>())
	{
		Platform->TiltPlatform(AngleValue);
	}
}

void ABasePlayerController::ServerHandleApprove_Implementation(ABasePlayerController* ApprovedController)
{
	OnStartMatchApproveDelegate.Broadcast(this);
}

void ABasePlayerController::Approve(const FInputActionValue& Value)
{
	if (IsLocalController() && Value.Get<bool>())
	{
		ServerHandleApprove(this);
	}
}

void ABasePlayerController::ClientUpdateScore_Implementation(const FText& NewScore)
{
	if (AMainHUD* MainHUD = GetHUD<AMainHUD>())
	{
		MainHUD->UpdateScorePannel(NewScore);
	}
}
