// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AimComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAimComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASE_MP_UE4_API IAimComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AimAt(FVector HitLocation) PURE_VIRTUAL(,);
};
