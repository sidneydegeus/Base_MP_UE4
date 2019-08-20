// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseVehicle.generated.h"

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


USTRUCT()
struct FVehicleState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FTransform Tranform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FVehicleMove LastMove;
};

UCLASS()
class BASE_MP_UE4_API ABaseVehicle : public APawn
{
	GENERATED_BODY()

//Variables
protected:
	float Throttle;
	float SteeringThrow;
	FVector Velocity;
	TArray<FVehicleMove> UnacknowledgedMoves;

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

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FVehicleState ServerState;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void SimulateMove(const FVehicleMove& Move);
	FVehicleMove CreateVehicleMove(float DeltaTime);
	void ClearAcknowledgedMoves(FVehicleMove LastMove);

	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);

	UFUNCTION()
		void OnRep_ServerState();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_SendMove(const FVehicleMove& Move);
		virtual void Server_SendMove_Implementation(const FVehicleMove& Move);
		virtual bool Server_SendMove_Validate(const FVehicleMove& Move);

};
