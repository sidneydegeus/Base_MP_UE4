// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Vehicle/Tank/TankBarrel.h"
#include "Vehicle/Tank/TankTurret.h"
#include "Kismet/GameplayStatics.h"

//TODO: rename tank barrel/turret classes to BarrelComponent / TurretComponent

void ATurret::BeginPlay() {
	Super::BeginPlay();
	//ActivateTick(false);
}

void ATurret::Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet) {
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}

FVector ATurret::SpawnProjectileLocation() {
	if (Barrel == nullptr) return FVector();
	return Barrel->GetSocketLocation(FName("Projectile"));
}

FRotator ATurret::SpawnProjectileRotation() {
	if (Barrel == nullptr) return FRotator();
	return Barrel->GetSocketRotation(FName("Projectile"));
}

void ATurret::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
		//FCollisionResponseParams::DefaultResponseParam,
		//TArray<AActor*>(),
		//true //debug draw
	);


	if (bHaveAimSolution) {
		CurrentAimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(CurrentAimDirection);
	}
}


/// Turret/Barrel component functionality
void ATurret::MoveBarrelTowards(FVector AimDirection) {
	if (GetOwner()->Role < ROLE_Authority) {
		Server_MoveBarrelTowards(AimDirection);
	}

	if (!ensure(Barrel || Turret)) return;

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else { // avoid going long-way around
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void ATurret::Server_MoveBarrelTowards_Implementation(FVector AimDirection) {
	MoveBarrelTowards(AimDirection);
}

bool ATurret::Server_MoveBarrelTowards_Validate(FVector AimDirection) {
	return true;
}

bool ATurret::IsBarrelMoving() {
	if (!ensure(Barrel)) return false;
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(CurrentAimDirection, 0.01);
}