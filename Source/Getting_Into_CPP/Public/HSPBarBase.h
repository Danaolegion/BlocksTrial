// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "HSPBarBase.generated.h"

/**
 * 
 */
class UStatBarBase;
UCLASS()
class GETTING_INTO_CPP_API UHSPBarBase : public UTrialWidgetBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly,Category="Constituent Controls",meta=(BindWidget))
	TObjectPtr<UStatBarBase> HealthBar= nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Constituent Controls",meta=(BindWidget))
	TObjectPtr<UStatBarBase> StaminaBar= nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Constituent Controls",meta=(BindWidget))
	TObjectPtr<UStatBarBase> PsiBar= nullptr;

	
	
};
