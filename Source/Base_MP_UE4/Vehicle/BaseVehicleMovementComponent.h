// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "BaseVehicleMovementComponent.generated.h"

USTRUCT()
struct FVehicleMove
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		float Throttle;
	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;
};

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASE_MP_UE4_API UBaseVehicleMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

// Variables
protected:
	float Throttle;
	float SteeringThrow;
	FVector Velocity;

	UPROPERTY(EditAnywhere)
		float Mass = 1000;

	// The force applied to the car when the throttle is fully down (N).
	UPROPERTY(EditAnywhere)
		float MaxDrivingForce = 10000;

	// Minimum radius of the car turning circle at full lock (m).
	UPROPERTY(EditAnywhere)
		float MinTurningRadius = 10;

	// Higher means more drag.
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 16;

	// Higher means more rolling resistance.
	UPROPERTY(EditAnywhere)
		float RollingResistanceCoefficient = 0.015;

// Functions
public:
	void SimulateMove(const FVehicleMove& Move);
	FVehicleMove CreateVehicleMove(float DeltaTime);

	FVector GetVelocity() const { return Velocity; };
	void SetVelocity(FVector Value) { Velocity = Value; };

	void SetThrottle(float Value) { Throttle = Value; };
	void SetSteeringThrow(float Value) { SteeringThrow = Value; };

protected:
	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);
	
};
