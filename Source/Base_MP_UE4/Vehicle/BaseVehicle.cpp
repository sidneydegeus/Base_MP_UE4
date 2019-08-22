// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Vehicle/BaseVehicleMovementComponent.h"

ABaseVehicle::ABaseVehicle() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = false;

	//TODO: do the following in a function that can be overridden by child classes
	// so that different movement components can be added 
	MovementComponent = CreateDefaultSubobject<UBaseVehicleMovementComponent>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UBaseVehicleMovementReplicator>(TEXT("MovementReplicator"));
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
}

void ABaseVehicle::MoveForward(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Throw);
}

void ABaseVehicle::MoveRight(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Throw);
}




