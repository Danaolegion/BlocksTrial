// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialCharacter.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TrialPlayerController.generated.h"

class ATrialHud;
class UInputAction;
class UInputMappingContext;
UCLASS(Abstract)
class GETTING_INTO_CPP_API ATrialPlayerController : public APlayerController
{
private:
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="playerInputs")
	TObjectPtr<UInputAction> ActionMove=nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="playerInputs")
	TObjectPtr<UInputAction> ActionLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="playerInputs")
	TObjectPtr<UInputAction> ActionJump = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="playerInputs")
	TObjectPtr<UInputAction> ActionCrouch=nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="playerInputs")
	TObjectPtr<UInputAction> ActionSprint=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="playerInputs")
	TObjectPtr<UInputAction> ActionPsiBlast=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="playerInputs")
	TObjectPtr<UInputAction> ActionCycleUIMode=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="playerInputs")
	TObjectPtr<UInputMappingContext> TrialMappingContext=nullptr;


protected:
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleCrouch();
	void HandleSprint();
	void HandlePsiBlast();
	void HandleCycleUIMode();

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY()
	TObjectPtr<ATrialCharacter> PlayerCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<ATrialHud> PlayerHud=nullptr;
};
