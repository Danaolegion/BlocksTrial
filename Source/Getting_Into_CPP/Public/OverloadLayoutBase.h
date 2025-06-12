// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrialWidgetBase.h"
#include "OverloadLayoutBase.generated.h"

/**
 * 
 */
class UHSPBarBase;
class UImage; 
UCLASS(Abstract)
class GETTING_INTO_CPP_API UOverloadLayoutBase : public UTrialWidgetBase
{
public:
	UPROPERTY(BlueprintReadOnly,Category="Constituent",meta=(BindWidget))
	TObjectPtr<UHSPBarBase> HSPBar =nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Constituent",meta=(BindWidget))
	TObjectPtr<UImage> Crosshair = nullptr;

private:
	GENERATED_BODY()
};
