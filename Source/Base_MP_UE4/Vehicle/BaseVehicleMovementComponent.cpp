// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicleMovementComponent.h"

#include "Engine/Engine.h"
#include "GameFrameWork/GameState.h"

UBaseVehicleMovementComponent::UBaseVehicleMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy) {
		LastMove = CreateVehicleMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UBaseVehicleMovementComponent::SimulateMove(const FVehicleMove& Move) {
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(Move.DeltaTime);
}

FVehicleMove UBaseVehicleMovementComponent::CreateVehicleMove(float DeltaTime) {
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = CurrentSteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	return Move;
}

FVector UBaseVehicleMovementComponent::GetAirResistance() {
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UBaseVehicleMovementComponent::GetRollingResistance() {
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UBaseVehicleMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrow) {
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UBaseVehicleMovementComponent::UpdateLocationFromVelocity(float DeltaTime) {
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult Hit;
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);
	if (Hit.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}