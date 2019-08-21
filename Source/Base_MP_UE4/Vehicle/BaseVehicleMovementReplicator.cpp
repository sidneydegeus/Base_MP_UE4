// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicleMovementReplicator.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void UBaseVehicleMovementReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseVehicleMovementReplicator, ServerState);
}

UBaseVehicleMovementReplicator::UBaseVehicleMovementReplicator() {
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UBaseVehicleMovementReplicator::BeginPlay() {
	Super::BeginPlay();
	MovementComponent = GetOwner()->FindComponentByClass<UBaseVehicleMovementComponent>();
}

void UBaseVehicleMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;
	FVehicleMove LastMove = MovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy) {
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}

	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy) {
		UpdateServerState(LastMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy) {
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void UBaseVehicleMovementReplicator::OnRep_ServerState() {
	if (MovementComponent == nullptr) return;
	GetOwner()->SetActorTransform(ServerState.Tranform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);
	for (const FVehicleMove& Move : UnacknowledgedMoves) {
		MovementComponent->SimulateMove(Move);
	}
}

void UBaseVehicleMovementReplicator::ClearAcknowledgedMoves(FVehicleMove LastMove) {
	TArray<FVehicleMove> NewMoves;
	for (const FVehicleMove& Move : UnacknowledgedMoves) {
		if (Move.Time > LastMove.Time) {
			NewMoves.Add(Move);
		}
	}
	UnacknowledgedMoves = NewMoves;
}

void UBaseVehicleMovementReplicator::UpdateServerState(const FVehicleMove& Move) {
	ServerState.LastMove = Move;
	ServerState.Tranform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UBaseVehicleMovementReplicator::Server_SendMove_Implementation(const FVehicleMove& Move) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

bool UBaseVehicleMovementReplicator::Server_SendMove_Validate(const FVehicleMove& Move) {
	return true; //TODO: Make better validation
}

