// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BarrelComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories = ("Collision"))
class BASE_MP_UE4_API UBarrelComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
//Variables
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 10;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxElevationDegrees = 40;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MinElevationDegrees = 0;

//Functions
public:
	void Elevate(float RelativeSpeed);
};
