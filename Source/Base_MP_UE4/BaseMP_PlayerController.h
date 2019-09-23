// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseAimingComponent.h"
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
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;

	UPROPERTY(Replicated)
	UBaseAimingComponent* AimingComponent;

//Functions
public:
	virtual void Tick(float DeltaTime) override;
	void SetAimingComponent(UBaseAimingComponent* Component) { AimingComponent = Component; };

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
};
