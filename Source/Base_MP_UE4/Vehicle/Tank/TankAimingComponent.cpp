// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Vehicle/Tank/TankBarrel.h"
#include "Vehicle/Tank/TankTurret.h"

void UTankAimingComponent::Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet) {
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}

void UTankAimingComponent::AimAt() {
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	//bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
	//	this,
	//	OutLaunchVelocity,
	//	StartLocation,
	//	HitLocation,
	//	LaunchSpeed,
	//	false,
	//	0,
	//	0,
	//	ESuggestProjVelocityTraceOption::DoNotTrace
	//	//FCollisionResponseParams::DefaultResponseParam,
	//	//TArray<AActor*>(),
	//	//true //debug draw
	//);


	//if (bHaveAimSolution) {
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	//}
}

UTankAimingComponent::UTankAimingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}
void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	//if (GetOwner()->Role < ROLE_Authority) {
	//	Server_MoveBarrelTowards(AimDirection);
	//}

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

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) return false;
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01);
}

