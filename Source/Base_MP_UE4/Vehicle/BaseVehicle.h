// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "Vehicle/BaseVehicleMovementReplicator.h"
#include "BaseVehicle.generated.h"

UCLASS()
class BASE_MP_UE4_API ABaseVehicle : public APawn
{
	GENERATED_BODY()

//Variables
protected:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* AzimuthGimbal;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UBaseVehicleMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
		UBaseVehicleMovementReplicator* MovementReplicator;

private:
	float TotalDeltaPitch;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void CreateMovementComponent();
	virtual void CreateMovementReplicator();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	virtual void RotateAzimuthGimbal(float Delta);
	virtual void ElevateSpringArm(float Delta);

private:
	void CreateCameraComponent();
};
