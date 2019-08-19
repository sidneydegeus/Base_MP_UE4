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
		FVehicleState  ServerState;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);

	UFUNCTION()
		void OnRep_ServerState();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_SendMove(FVehicleMove Move);
		virtual void Server_SendMove_Implementation(FVehicleMove Move);
		virtual bool Server_SendMove_Validate(FVehicleMove Move);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_MoveRight(float Throw);
		virtual void Server_MoveRight_Implementation(float Throw);
		virtual bool Server_MoveRight_Validate(float Throw);
};
