// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialHud.h"
#include "ModerateLayoutBase.h"
#include "MinimalLayoutBase.h"
#include "OverloadLayoutBase.h"
#include "TrialCharacter.h"
#include "HSPBarBase.h"
#include "StatBarBase.h"
#include "Components/SlateWrapperTypes.h"


void ATrialHud::CycleToNextViewMode()
{
	++CurrentViewMode;
	UE_LOG(LogTemp,Warning,TEXT("CycleToNextViewMode %s"),*UEnum::GetValueAsString(CurrentViewMode))
	UpdateWidgets();
}

void ATrialHud::BeginPlay()
{
	Super::BeginPlay();
	//Get a refrence to the current world object
	World =GetWorld();
	checkf(World,TEXT("Failed to refrence the World"))

	//Ensure we have valid values for 3 classes of widget used by the HUD
	checkf(MinimalLayoutClass,TEXT("Invalid MinimalLayoutClass reference"))
	checkf(ModerateLayoutClass,TEXT("Invalid ModerateLayoutClass reference"))
	checkf(OverloadLayoutClass,TEXT("Invalid OverloadLayoutClass reference"))

	//Create the 3 types of Widget and add them to the Viewport
	//We could have been clever and had a single widget which mutates
	//When creating the widget first parameter(Owning Object) must be
	//of following types: UWidget,UWidgetTree,APlayerController,UGameInstance
	//or UWorld.

	MinimalLayoutWidget = CreateWidget<UMinimalLayoutBase>(World,MinimalLayoutClass);
	MinimalLayoutWidget->AddToViewport();
	MinimalLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	ModerateLayoutWidget = CreateWidget<UModerateLayoutBase>(World,ModerateLayoutClass);
	ModerateLayoutWidget->AddToViewport();
	ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	OverloadLayoutWidget= CreateWidget<UOverloadLayoutBase>(World,OverloadLayoutClass);
	OverloadLayoutWidget->AddToViewport();
	OverloadLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	//Get a reference to the character, and hook up the stat handler
	if(APlayerController* PlayerController=GetOwningPlayerController())
	{
		PlayerCharacter=Cast<ATrialCharacter>(PlayerController->GetPawn());
	}
	checkf(PlayerCharacter,TEXT("Unable to reference to the player Character"))

	//Set the initial Viewmode to the 'Current' One ,
	//Which allows setting via the editor
	//SetCurrentViewMode(CurrentViewMode)
	UpdateWidgets();
}

void ATrialHud::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATrialHud::UpdateWidgets()
{
	//Unhook any delegate handlers
	ClearAllHandlers();

	MinimalLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);
	ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);
	OverloadLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	switch (CurrentViewMode)
	{
	case EHudViewMode::CleanAndPristine:
		//currently there isn't actually anything to do here!
			break;
	case EHudViewMode::Minimal:
		PlayerCharacter->OnHealthChanged.AddDynamic(MinimalLayoutWidget->HSPBar->HealthBar,
													&UStatBarBase::OnIntStatUpdated);
		PlayerCharacter->OnStaminaChanged.AddDynamic(MinimalLayoutWidget->HSPBar->StaminaBar,
													&UStatBarBase::OnFloatStatUpdated);
		PlayerCharacter->OnPsiPowerChanged.AddDynamic(MinimalLayoutWidget->HSPBar->PsiBar,
													&UStatBarBase::OnFloatStatUpdated);
		MinimalLayoutWidget->SetVisibility(ESlateVisibility::Visible);
		break;
	case EHudViewMode::Moderate:
		PlayerCharacter->OnHealthChanged.AddDynamic(ModerateLayoutWidget->HSPBar->HealthBar,
													&UStatBarBase::OnIntStatUpdated);
		PlayerCharacter->OnStaminaChanged.AddDynamic(ModerateLayoutWidget->HSPBar->StaminaBar,
													&UStatBarBase::OnFloatStatUpdated);
		PlayerCharacter->OnPsiPowerChanged.AddDynamic(ModerateLayoutWidget->HSPBar->PsiBar,
													&UStatBarBase::OnFloatStatUpdated);
		ModerateLayoutWidget->SetVisibility(ESlateVisibility::Visible);
		break;
	case EHudViewMode::SensoryOverload:
		PlayerCharacter->OnHealthChanged.AddDynamic(OverloadLayoutWidget->HSPBar->HealthBar,
													&UStatBarBase::OnIntStatUpdated);
		PlayerCharacter->OnStaminaChanged.AddDynamic(OverloadLayoutWidget->HSPBar->StaminaBar,
													&UStatBarBase::OnFloatStatUpdated);
		PlayerCharacter->OnPsiPowerChanged.AddDynamic(OverloadLayoutWidget->HSPBar->PsiBar,
													&UStatBarBase::OnFloatStatUpdated);
		ModerateLayoutWidget->SetVisibility(ESlateVisibility::Visible);
	default: ;
	}
	//This ensures that even if something has not changed recently , the newly switched to widget
	//will get sent the latest character stats, so it can update itself.
	PlayerCharacter->BroadcastCurrentStats();
}

void ATrialHud::ClearAllHandlers()
{
	if(PlayerCharacter)
	{
		PlayerCharacter->OnHealthChanged.Clear();
		PlayerCharacter->OnStaminaChanged.Clear();
		PlayerCharacter->OnPsiPowerChanged.Clear();
		PlayerCharacter->OnKeyWalletAction.Clear();
	}
}
