// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AimComponentInterface.h"
#include "BaseMP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ABaseMP_PlayerController : public APlayerController
{
	GENERATED_BODY()

//Variables
private:

//Functions
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	//void AimTowardsCrosshair();
	//bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	//bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	//bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
};
