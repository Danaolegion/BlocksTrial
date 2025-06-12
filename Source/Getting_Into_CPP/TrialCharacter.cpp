// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ATrialCharacter::ATrialCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.5f);
	AActor::SetActorTickEnabled(true);
}

void ATrialCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if(bIsRunning &&CurrentStamina<=0)
	{
		SetRunning(false);
	}
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	//set the flag to indicate the character ran
	if(bIsRunning) bHasRun=true;
}

void ATrialCharacter::Jump()
{
	//jump require stamina
	if(CurrentStamina-JumpStaminaCost >=0.f)
	{
		UnCrouch();
		Super::Jump();
		bHasJumped=true;
	}
	
}

void ATrialCharacter::Crouch(bool bClientSimulation)
{
	SetRunning(false);
	Super::Crouch(bClientSimulation);
}


void ATrialCharacter::BroadcastCurrentStats()
{
	OnHealthChanged.Broadcast(CurrentHealth,CurrentHealth,MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina,CurrentStamina,MaxStamina);
	OnHealthChanged.Broadcast(CurrentPsiPower,CurrentPsiPower,MaxPsiPower);

	//Make a string of all the keys
	//if there are any members, the string will end with a trailing comma ","
	//We don't care to remove that here , it doesn't matter
	FString AllKeys =FString();
	for (FString Key : KeyWallet)
	{
		AllKeys.Appendf(TEXT("%s,"),&Key);
	}

	OnKeyWalletAction.Broadcast(AllKeys,EPLayerKeyAction::CountKey,true);
}

// Called when the game starts or when spawned
void ATrialCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Two things which can restore overTime i.e 1.Stamina 2.PsiPower

#pragma region Update Stamina
	
	//How will the stamina get affected
	//if they jump,otherwise if the run ,or if they rested
	//and if they did none they get the default recharge rate;
	float ActualStaminaRecuperatorFactor=StaminaRecuperationFactor;

	//if jump it will cost stamina
	if(bHasJumped) ActualStaminaRecuperatorFactor = -JumpStaminaCost;

	//if they run it will cost stamina
	else if (bHasRun) ActualStaminaRecuperatorFactor = -RunStaminaCost;

	//if they rest the stamina will restore
	else if(bIsCrouched) ActualStaminaRecuperatorFactor = RestStaminaRebate;

	//Keep tracks of the value ,before it is changed
	const float PreviousStamina= CurrentStamina;

	//keeping  the current value in limit
	CurrentStamina =FMath::Clamp(CurrentStamina+ActualStaminaRecuperatorFactor,0.f,MaxStamina);

	//if the values have actually changed we t
	if(CurrentStamina !=PreviousStamina)
	{
		OnStaminaChanged.Broadcast(PreviousStamina,CurrentStamina,MaxStamina);
	}

	//Reset the flags indicationg plysical exertion
	bHasRun=false;
	bHasJumped=false;

#pragma endregion

#pragma region Update Psi Power

	//We only have to restore it and at constant rate

	if(CurrentPsiPower!=MaxPsiPower)
	{
		const float PreviousPsiPower =CurrentPsiPower;

		CurrentPsiPower=FMath::Clamp(PreviousPsiPower+PsiRechargeRate,0,MaxPsiPower);
		OnPsiPowerChanged.Broadcast(PreviousPsiPower,CurrentPsiPower,MaxPsiPower);
	}
#pragma endregion

	GEngine->AddOnScreenDebugMessage(-1,0.49f,FColor::Silver,
										*(FString::Printf(TEXT("Movement- IsCrouched:%d | IsSprinting:%d"),bIsCrouched,bIsRunning)));
	GEngine->AddOnScreenDebugMessage(-1,0.49f,FColor::Red,
									*(FString::Printf(TEXT("Health- Current:%d | Maximum:%d"),CurrentHealth,MaxHealth)));
	GEngine->AddOnScreenDebugMessage(-1,0.49f,FColor::Green,
									*(FString::Printf(TEXT("Stamina- Current:%f | Maximum:%f"),CurrentStamina,MaxStamina)));
	GEngine->AddOnScreenDebugMessage(-1,0.49f,FColor::Cyan,
									*(FString::Printf(TEXT("PsiPower- Current:%f | Maximum:%f"),CurrentPsiPower,MaxPsiPower)));
	GEngine->AddOnScreenDebugMessage(-1,0.49f,FColor::Orange,
									*(FString::Printf(TEXT("Keys - %d keys Currently held"),KeyWallet.Num())));  
}

// Called to bind functionality to input
void ATrialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATrialCharacter::SetRunning(bool IsRunning)
{
	bIsRunning=IsRunning;

	GetCharacterMovement()->MaxWalkSpeed =bIsRunning ? RunningMaxWalkSpeed : NormalWalkMaxSpeed;
}

void ATrialCharacter::ToggleSprint()
{
	SetRunning(!bIsRunning);
}

int ATrialCharacter::GetHealth()
{
	return CurrentHealth;
}

int ATrialCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void ATrialCharacter::UpdateHealth(int DeltaHealth)
{
    //agar player marta hai to uski health modify nahi karsakte ye prevent karne ke liye
	//Ye prevent karta ha multiple stacking agar ye nahi kiya to player marta hai fir se
	//jinda ho jata hai this below line of code avoid that
	if(CurrentHealth <=0.f) return;

	// what is the value before we change it ?
	int OldValue = CurrentHealth;
	
	CurrentHealth +=DeltaHealth;

	//Have to make sure ki current health should be in acceptible range so uske liye clamp
	//kare
	CurrentHealth =FMath::Clamp(CurrentHealth,-1,MaxHealth);

	if(CurrentHealth !=OldValue)
	{
		OnHealthChanged.Broadcast(OldValue,CurrentHealth,MaxHealth);
	}
	if(CurrentHealth <=0.f)
	{
		OnPlayerDied.Broadcast();
	}
}

void ATrialCharacter::RestoreToFullHealth()
{
	if(CurrentHealth < MaxHealth)
	{
		int OldValue =CurrentHealth;
		CurrentHealth=MaxHealth;
		OnHealthChanged.Broadcast(OldValue,CurrentHealth,MaxHealth);
	}	
}

void ATrialCharacter::SetMaxHealth(int NewMaxHealth)
{
	//We have to assume tha new value is within the acceptible range
	// acha hoga are hum range check kar sake
	MaxHealth =NewMaxHealth;
}



float ATrialCharacter::GetStamina()
{
	return CurrentStamina;
}

float ATrialCharacter::GetStaminaRecuperationFactor()
{
	return StaminaRecuperationFactor;
}

void ATrialCharacter::SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor)
{
	//Might be sensible to check that this is +ve value, within some sensible range
	StaminaRecuperationFactor = NewStaminaRecuperationFactor;
}

float ATrialCharacter::GetPsiPower()
{
	return CurrentPsiPower;
}

void ATrialCharacter::PsiBlast()
{
	//check karna hai ku Psiblast use karne keliye enough power hai kya
	if(CurrentPsiPower >= PsiBlastCost)
	{
		//Do something with psiblast

		//Deduct the cost from the current power
		CurrentPsiPower -= PsiBlastCost;
	}
}

void ATrialCharacter::AddKey(FString KeyToAdd)
{
	if(KeyWallet.Contains(KeyToAdd))
	{
		//Key already in there,play a noise
	}
	else
	{
		KeyWallet.Add(KeyToAdd);
	}
}

void ATrialCharacter::RemoveKey(FString KeyToRemove)
{
	if(KeyWallet.Contains(KeyToRemove))
	{
		
	}
}

bool ATrialCharacter::IsPlayerCarryingKey(FString DesiredKey)
{
	return KeyWallet.Contains(DesiredKey);
}
