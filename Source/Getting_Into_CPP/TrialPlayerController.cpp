// Fill out your copyright notice in the Description page of Project Settings.

#include "TrialPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "TrialHud.h"

void ATrialPlayerController::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementValue.Y,false);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementValue.X,false);
	}
}

void ATrialPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue= Value.Get<FVector2D>();

	AddYawInput(LookAxisValue.X);
	AddPitchInput(LookAxisValue.Y);
}

void ATrialPlayerController::HandleJump()
{
	if(PlayerCharacter)
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
}

void ATrialPlayerController::HandleCrouch()
{
	if(PlayerCharacter && PlayerCharacter->bIsCrouched)
	{
		PlayerCharacter->UnCrouch(); 
	}
	else
	{
		PlayerCharacter->Crouch(false);
	}
}

void ATrialPlayerController::HandleSprint()
{
	if(PlayerCharacter) PlayerCharacter->ToggleSprint();
}

void ATrialPlayerController::HandlePsiBlast()
{
	if(PlayerCharacter) PlayerCharacter->PsiBlast();
}

void ATrialPlayerController::HandleCycleUIMode()
{
	if(PlayerHud)
	{
		PlayerHud->CycleToNextViewMode();
	}
}

void ATrialPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//Store a refrence to the player pawn (ATrialCharacter cast ho raha ha aPawn ke sath)
	PlayerCharacter = Cast<ATrialCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("ATrialPlayerController  derived classes	should only possed ATrialCharacter"))

	//Get a refrence to the Enhanced Input Component
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get refrence to EnhancedInputComponent"))

	if (ActionMove)
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this,
		                                   &ATrialPlayerController::HandleMove);

	if (ActionLook)
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,
		                                   &ATrialPlayerController::HandleLook);

	if (ActionJump)
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this,
		                                   &ATrialPlayerController::HandleJump);

	if(ActionCrouch)
		EnhancedInputComponent->BindAction(ActionCrouch,ETriggerEvent::Completed,this,
		                                   &ATrialPlayerController::HandleCrouch);

	if(ActionSprint)
		EnhancedInputComponent->BindAction(ActionSprint,ETriggerEvent::Completed,this,
										   &ATrialPlayerController::HandleSprint);

	if(ActionPsiBlast)
		EnhancedInputComponent->BindAction(ActionPsiBlast,ETriggerEvent::Completed,this,
										   &ATrialPlayerController::HandlePsiBlast);
	if(ActionCycleUIMode)
		EnhancedInputComponent->BindAction(ActionCycleUIMode,ETriggerEvent::Triggered,this,
										   &ATrialPlayerController::HandleCycleUIMode);

	UEnhancedInputLocalPlayerSubsystem* Subsystem=
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(Subsystem,TEXT("Unable to get refrence to the EnhancedInputLocalPlayerSubsystem"))

	checkf(TrialMappingContext,TEXT("IMC was not spicified"))
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(TrialMappingContext,0);
}

void ATrialPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	EnhancedInputComponent->ClearActionBindings();
}
