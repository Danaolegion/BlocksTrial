// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueprintsFunctionlibrary.generated.h"

UCLASS()
class GETTING_INTO_CPP_API ABlueprintsFunctionlibrary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueprintsFunctionlibrary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static FString GetNormalString();

	static  constexpr double Pi = 3.141592653589793238462643383279502884197169399;

	UFUNCTION(BlueprintCallable,BlueprintPure,meta=(CompactNodeTitle="PI"))
	static double GetMePi();

};
