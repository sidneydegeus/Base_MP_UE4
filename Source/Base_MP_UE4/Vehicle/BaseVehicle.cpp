// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"

void ABaseVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseVehicle, ServerState);
	DOREPLIFETIME(ABaseVehicle, Throttle);
	DOREPLIFETIME(ABaseVehicle, SteeringThrow);
}

ABaseVehicle::ABaseVehicle() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);

	UpdateLocationFromVelocity(DeltaTime);

	if (HasAuthority()) {
		ServerState.Tranform = GetActorTransform();
		ServerState.Velocity = Velocity;
		//TODO: Update last move
	}

	//DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}

void ABaseVehicle::OnRep_ServerState() {
	SetActorTransform(ServerState.Tranform);
	Velocity = ServerState.Velocity;
}

FVector ABaseVehicle::GetAirResistance() {
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector ABaseVehicle::GetRollingResistance() {
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}


void ABaseVehicle::ApplyRotation(float DeltaTime) {
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void ABaseVehicle::UpdateLocationFromVelocity(float DeltaTime) {
	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult Hit;
	AddActorWorldOffset(Translation, true, &Hit);
	if (Hit.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

void ABaseVehicle::MoveForward(float Throw) {
	Throttle = Throw;
}

void ABaseVehicle::MoveRight(float Throw) {
	SteeringThrow = Throw;
}

void ABaseVehicle::Server_SendMove_Implementation(FVehicleMove Move) {
	Throttle = Move.Throttle;
	SteeringThrow = Move.SteeringThrow;
}

bool ABaseVehicle::Server_SendMove_Validate(FVehicleMove Move) {
	return true; //TODO: Make better validation
}



