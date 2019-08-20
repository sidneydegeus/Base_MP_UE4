// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "BaseVehicle.generated.h"

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
	TArray<FVehicleMove> UnacknowledgedMoves;	

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FVehicleState ServerState;

	UPROPERTY(EditAnywhere)
		UBaseVehicleMovementComponent* MovementComponent;

//Functions 
public:	
	ABaseVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void ClearAcknowledgedMoves(FVehicleMove LastMove);

	UFUNCTION()
		void OnRep_ServerState();

	virtual void MoveForward(float Throw);
	virtual void MoveRight(float Throw);

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_SendMove(const FVehicleMove& Move);
		virtual void Server_SendMove_Implementation(const FVehicleMove& Move);
		virtual bool Server_SendMove_Validate(const FVehicleMove& Move);

};
