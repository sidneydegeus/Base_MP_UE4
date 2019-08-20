// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Vehicle/BaseVehicleMovementComponent.h"

void ABaseVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseVehicle, ServerState);
}

ABaseVehicle::ABaseVehicle() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//TODO: do the following in a function that can be overridden by child classes
	// so that different movement components can be added 
	MovementComponent = CreateDefaultSubobject<UBaseVehicleMovementComponent>(TEXT("MovementComponent"));
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);
}

void ABaseVehicle::BeginPlay() {
	Super::BeginPlay();
}

void ABaseVehicle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (MovementComponent == nullptr) return;

	if (Role == ROLE_AutonomousProxy) {
		FVehicleMove Move = MovementComponent->CreateVehicleMove(DeltaTime);
		MovementComponent->SimulateMove(Move);
		UnacknowledgedMoves.Add(Move);
		Server_SendMove(Move);
	}

	if (Role == ROLE_Authority && GetRemoteRole() == ROLE_SimulatedProxy) {
		FVehicleMove Move = MovementComponent->CreateVehicleMove(DeltaTime);
		Server_SendMove(Move);
	}

	if (Role == ROLE_SimulatedProxy) {
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void ABaseVehicle::OnRep_ServerState() {
	if (MovementComponent == nullptr) return;
	SetActorTransform(ServerState.Tranform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);
	for (const FVehicleMove& Move : UnacknowledgedMoves) {
		MovementComponent->SimulateMove(Move);
	}
}

void ABaseVehicle::ClearAcknowledgedMoves(FVehicleMove LastMove) {
	TArray<FVehicleMove> NewMoves;
	for (const FVehicleMove& Move : UnacknowledgedMoves) {
		if (Move.Time > LastMove.Time) {
			NewMoves.Add(Move);
		}
	}
	UnacknowledgedMoves = NewMoves;
}

void ABaseVehicle::MoveForward(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Throw);
}

void ABaseVehicle::MoveRight(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Throw);
}

void ABaseVehicle::Server_SendMove_Implementation(const FVehicleMove& Move) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SimulateMove(Move);

	ServerState.LastMove = Move;
	ServerState.Tranform = GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

bool ABaseVehicle::Server_SendMove_Validate(const FVehicleMove& Move) {
	return true; //TODO: Make better validation
}



