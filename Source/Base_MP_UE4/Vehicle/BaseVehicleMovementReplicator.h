// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "BaseVehicleMovementReplicator.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API UBaseVehicleMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

//Variables
private:
	TArray<FVehicleMove> UnacknowledgedMoves;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FVehicleState ServerState;

	UPROPERTY()
		UBaseVehicleMovementComponent* MovementComponent;

//Functions
public:	
	// Sets default values for this component's properties
	UBaseVehicleMovementReplicator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	void ClearAcknowledgedMoves(FVehicleMove LastMove);
	void UpdateServerState(const FVehicleMove& Move);

	UFUNCTION()
		void OnRep_ServerState();

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_SendMove(const FVehicleMove& Move);
		virtual void Server_SendMove_Implementation(const FVehicleMove& Move);
		virtual bool Server_SendMove_Validate(const FVehicleMove& Move);

};
