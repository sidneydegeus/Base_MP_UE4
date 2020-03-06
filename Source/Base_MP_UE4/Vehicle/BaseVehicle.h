// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedPawn.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "Vehicle/BaseVehicleMovementReplicator.h"
#include "BaseVehicle.generated.h"

UCLASS()
class BASE_MP_UE4_API ABaseVehicle : public AExtendedPawn
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

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CreateMovementComponent();
	virtual void CreateMovementReplicator();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;



};
