// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "Vehicle/BaseVehicleMovementReplicator.h"
#include "GenericComponents/ExitPawnComponent.h"
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
	UPROPERTY(VisibleAnywhere)
		UExitPawnComponent* ExitComponent;


	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> VehicleUIClass;

	UPROPERTY()
		UUserWidget* VehicleUI;



private:
	float TotalDeltaPitch;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ExitVehicle();

protected:
	virtual void BeginPlay() override;
	virtual void CreateMovementComponent();
	virtual void CreateMovementReplicator();
	virtual void CreateExitComponent();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	virtual void RotateAzimuthGimbal(float Delta);
	virtual void ElevateSpringArm(float Delta);

	virtual void PossessedBy(AController* NewController) override;
	UFUNCTION(Client, Reliable)
		void Client_PossessedBy();
		void Client_PossessedBy_Implementation();

	virtual void UnPossessed() override;
	UFUNCTION(Client, Reliable)
		void Client_UnPossessed();
		void Client_UnPossessed_Implementation();

private:
	void CreateCameraComponent();
};
