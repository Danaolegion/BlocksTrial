// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrialCharacter.generated.h"

//Delegate for when stats based on integers are changed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
												int32,OldValue,
												int32,NewValue,
												int32,MaxValue);

//Delegate for when the player Dies
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

//Delegate for When stats based on float are changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
												float,OldValue,
												float,NewValue,
												float,MaxValue);
//Different Actios involving the key wallet.
UENUM(BlueprintType)
enum class  EPLayerKeyAction:uint8
{
	AddKey UMETA(Tooltip ="Attempt to add key to player's wallet."),
	RemoveKey UMETA(Tooltip ="Attempt to Remove key from player's wallet."),
	TestKey UMETA(Tooltip ="Check if the player has a specific key ."),
	CountKey UMETA(Tooltip ="How many keys in collection")
};

//Delegate for when the action happens with the players key
//Keystring-: is the key involved in action
//KeyAction-: is shown what was attempted.
//IsSuccess-: os shown if the attempted action happened or didn't.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FKeyWalletAction,
												FString,KeyString,
												EPLayerKeyAction,KeyAction,
												bool,IsSuccess);
UCLASS()
class GETTING_INTO_CPP_API ATrialCharacter : public ACharacter
{

public:
	// Sets default values for this character's properties
	ATrialCharacter();

	//OverRiding the  Methods
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	virtual void Jump() override;

	virtual void Crouch(bool bClientSimulation) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Sprint 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PlayerMovement")
	float NormalWalkMaxSpeed= 400.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerMovement")
	float RunningMaxWalkSpeed= 800.f;

	UFUNCTION(BlueprintCallable,Category=PlayerMovement)
	void SetRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable,Category=PlayerMovement)
	void ToggleSprint();
#pragma endregion
	
#pragma region Health
	
	//return the Players Current Health
	UFUNCTION(BlueprintPure,Category="PlayerHealth")
	int GetHealth();

	//Return MaxHealth of player
	UFUNCTION(BlueprintPure,Category="PlayerHealth")
	int GetMaxHealth();

	//Modify the player's Health by the specified amount -ve values are subtracted
	//+ve value are added
	UFUNCTION(BlueprintCallable,Category=PlayerHealth)
	void UpdateHealth(int DeltaHealth);

	//Sets player's current Health to maximum allowable
	UFUNCTION(BlueprintCallable,Category="PlayerHealth")
	void RestoreToFullHealth();

	//Set the Maximum allowable Health
	UFUNCTION(BlueprintCallable,Category="PlayerHealth")
	void SetMaxHealth(int NewMaxHealth);

	//Triggers when the player's health is changed
	UPROPERTY(BlueprintAssignable,Category="PlayerHealth")
	FIntStatUpdated OnHealthChanged;
	
	//Triggers when the player is Died
	UPROPERTY(BlueprintAssignable,Category="PlayerHealth")
	FPlayerIsDead OnPlayerDied;
#pragma endregion
	
#pragma region Stamina

	//Return The Player's Current Stamina
	UFUNCTION(BlueprintPure,Category="PLayerStamina")
	float GetStamina();

	//Return the players Current Recuperation factor.
	UFUNCTION(BlueprintPure,Category="PlayerStamina")
	float GetStaminaRecuperationFactor();

	UFUNCTION(BlueprintCallable,Category="PlayerStamina")
	void SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor);

	UPROPERTY(BlueprintAssignable,Category="PlayerStamina")
	FFloatStatUpdated OnStaminaChanged;
#pragma endregion	
	
#pragma region PsiPower
	//Returns the Player's current PsiPower
	UFUNCTION(BlueprintCallable,Category="PlayerPsiPower")
	float GetPsiPower();

	//Player unleashes a devastating blast of Mind Power!
	UFUNCTION(BlueprintCallable,Category="PlayerPsiPower")
	void PsiBlast();

	UPROPERTY(BlueprintAssignable,Category="PlayerPsiPower")
	FFloatStatUpdated OnPsiPowerChanged;
#pragma endregion

	
#pragma region KeyWallet
	//Add a Key to the wallet if it isn't already in there,
	//If it is already in there ,don't do anything
	UFUNCTION(BlueprintCallable,Category=PlayerKeyWallet)
	void AddKey(FString KeyToAdd);

	//Remove a key , if the key isn't in the wallet ,we do nothing
	UFUNCTION(BlueprintCallable,Category="PlayerKeyWallet")
	void RemoveKey(FString KeyToRemove);

	//Does the player have the Desired key ?
	//Return True if they do and false if they don't
	UFUNCTION(BlueprintPure,Category="PlayerKeyWallet")
	bool IsPlayerCarryingKey(FString DesiredKey);

	UPROPERTY(BlueprintAssignable, Category="PlayerKeyWallet")
	FKeyWalletAction OnKeyWalletAction;
#pragma endregion

	UFUNCTION(BlueprintCallable,Category="PlayerStats")
	void BroadcastCurrentStats();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

	// is the character current set to sprint
	bool bIsRunning=false;

	//did the character sprint since the last update
	bool bHasRun=false;

	//did the character Jump since the last update
	bool bHasJumped=false;
	//Health
	static constexpr int BaseStatValue = 100;
	int                  MaxHealth     = BaseStatValue;
	int                  CurrentHealth = BaseStatValue;

	//Stamina
	static constexpr float MaxStamina       = 100.f;
	static constexpr float JumpStaminaCost  = 25.f;
	static constexpr float RunStaminaCost   = 5.f;
	static constexpr float RestStaminaRebate= 4.f;
	float                  CurrentStamina   = MaxStamina;
	float        StaminaRecuperationFactor = 1.f;

	//Psi Power
	static constexpr float  MaxPsiPower    = 1000.f;
	static constexpr float PsiRechargeRate = 1.f;
	static constexpr float PsiBlastCost    =150.f;
	float                  CurrentPsiPower =MaxPsiPower;

	//Player Keys
	TArray<FString> KeyWallet;
};
