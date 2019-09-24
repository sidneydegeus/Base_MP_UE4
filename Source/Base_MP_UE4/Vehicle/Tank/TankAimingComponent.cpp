// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Vehicle/Tank/TankBarrel.h"
#include "Vehicle/Tank/TankTurret.h"
#include "Net/UnrealNetwork.h"

void UTankAimingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTankAimingComponent, TankFiringState);
}

UTankAimingComponent::UTankAimingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicated(true);
}

void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if (GetOwner()->Role == ROLE_Authority) {
	//	if (Ammo <= 0) {
	//		TankFiringState = ETankFiringState::NoAmmo;
	//	}
	//	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
	//		TankFiringState = ETankFiringState::Reloading;
	//	}
	//	else if (IsBarrelMoving()) {
	//		TankFiringState = ETankFiringState::Aiming;
	//	}
	//	else {
	//		TankFiringState = ETankFiringState::Locked;
	//	}
	//}
}

void UTankAimingComponent::Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet) {
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}




void UTankAimingComponent::AimAt(FVector HitLocation) {
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

void UTankAimingComponent::Fire() {
	//if (TankFiringState == ETankFiringState::Locked || TankFiringState == ETankFiringState::Aiming) {
	//	Server_Fire();
	//	TankFiringState = ETankFiringState::Reloading;
	//}
}


FVector UTankAimingComponent::SpawnProjectileLocation() {
	return Barrel->GetSocketLocation(FName("Projectile"));
}

FRotator UTankAimingComponent::SpawnProjectileRotation() {
	return Barrel->GetSocketRotation(FName("Projectile"));
}



/// Barrel functionality
void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
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

void UTankAimingComponent::Server_MoveBarrelTowards_Implementation(FVector AimDirection) {
	MoveBarrelTowards(AimDirection);
}

bool UTankAimingComponent::Server_MoveBarrelTowards_Validate(FVector AimDirection) {
	return true;
}

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) return false;
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(CurrentAimDirection, 0.01);
}



