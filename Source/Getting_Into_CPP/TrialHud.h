// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TrialHud.generated.h"

/**
 * 
 */
class ATrialCharacter;
class UMinimalLayoutBase;
class UModerateLayoutBase;
class UOverloadLayoutBase;
UENUM(BlueprintType)
enum class EHudViewMode: uint8
{
	CleanAndPristine UMETA(Tooltip="Get the mess out of my face!"),
	Minimal UMETA(Tooltip="Just the facts,Maam."),
	Moderate UMETA(Tooltip="Keep me well informed"),
	SensoryOverload UMETA(Tooltip="My other UI is a derivative trading screen")
};
inline EHudViewMode& operator++(EHudViewMode& ViewMode)
{
	if(ViewMode == EHudViewMode::SensoryOverload)
		ViewMode = EHudViewMode::CleanAndPristine;
	else
	{
		ViewMode =static_cast<EHudViewMode>(static_cast<int>(ViewMode)+1);
	}

	return ViewMode;
}
UCLASS(Abstract)
class GETTING_INTO_CPP_API ATrialHud : public AHUD
{
public:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMinimalLayoutBase>  MinimalLayoutClass = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UModerateLayoutBase>  ModerateLayoutClass = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverloadLayoutBase>  OverloadLayoutClass = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void CycleToNextViewMode();

protected:
	virtual void  BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	
	UPROPERTY()
	TObjectPtr<UWorld> World = nullptr;
	UPROPERTY(EditAnywhere)//Determine what UI elements should be displayed
	EHudViewMode CurrentViewMode = EHudViewMode::Minimal ;

	UPROPERTY()
	TObjectPtr<UMinimalLayoutBase> MinimalLayoutWidget= nullptr;

	UPROPERTY()
	TObjectPtr<UModerateLayoutBase> ModerateLayoutWidget =nullptr;

	UPROPERTY()
	TObjectPtr<UOverloadLayoutBase> OverloadLayoutWidget=nullptr;

	UPROPERTY()
	TObjectPtr<ATrialCharacter> PlayerCharacter=nullptr;

	//Whenever we change the viewmode , this private function is call
	//to show the appropriate widget 
	void UpdateWidgets();

	//Release any delegate binding
	void ClearAllHandlers();

	GENERATED_BODY()
};
