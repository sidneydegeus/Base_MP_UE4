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
		ClientTick(DeltaTime);
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
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UBaseVehicleMovementReplicator::ClientTick(float DeltaTime) {
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	FVector StartLocation = ClientStartTransform.GetLocation();
	FVector TargetLocation = ServerState.Transform.GetLocation();

	float VelocityToDerivative = ClientTimeBetweenLastUpdates * 100;
	FVector StartDerivative = ClientStartVelocity * VelocityToDerivative;
	FVector TargetDerivative = ServerState.Velocity * VelocityToDerivative;
	
	FVector NewLocation = FMath::CubicInterp(
		StartLocation, 
		StartDerivative, 
		TargetLocation, 
		TargetDerivative, 
		LerpRatio
	);
	GetOwner()->SetActorLocation(NewLocation);

	FVector NewDerivative = FMath::CubicInterpDerivative(
		StartLocation,
		StartDerivative,
		TargetLocation,
		TargetDerivative,
		LerpRatio
	);
	FVector NewVelocity = NewDerivative / VelocityToDerivative;
	MovementComponent->SetVelocity(NewVelocity);

	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	GetOwner()->SetActorRotation(NewRotation);
}

void UBaseVehicleMovementReplicator::OnRep_ServerState() {
	if (GetOwnerRole() == ROLE_AutonomousProxy) {
		AutonomousProxy_OnRep_ServerState();
	}
	else if (GetOwnerRole() == ROLE_SimulatedProxy) {
		SimulatedProxy_OnRep_ServerState();
	}

}

void UBaseVehicleMovementReplicator::SimulatedProxy_OnRep_ServerState() {
	if (MovementComponent == nullptr) return;
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	ClientStartTransform = GetOwner()->GetActorTransform();
	ClientStartVelocity = MovementComponent->GetVelocity();
}

void UBaseVehicleMovementReplicator::AutonomousProxy_OnRep_ServerState() {
	if (MovementComponent == nullptr) return;
	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);
	for (const FVehicleMove& Move : UnacknowledgedMoves) {
		MovementComponent->SimulateMove(Move);
	}
}

void UBaseVehicleMovementReplicator::Server_SendMove_Implementation(const FVehicleMove& Move) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

bool UBaseVehicleMovementReplicator::Server_SendMove_Validate(const FVehicleMove& Move) {
	return true; //TODO: Make better validation
}

