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
		UBaseVehicleMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
		UBaseVehicleMovementReplicator* MovementReplicator;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

};
