// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintsFunctionlibrary.h"

// Sets default values
ABlueprintsFunctionlibrary::ABlueprintsFunctionlibrary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlueprintsFunctionlibrary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlueprintsFunctionlibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABlueprintsFunctionlibrary::GetNormalString()
{
	return FString("This is useless string funtiong");
}

double ABlueprintsFunctionlibrary::GetMePi()
{
	return Pi;
}

