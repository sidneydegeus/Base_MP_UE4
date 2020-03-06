// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Weapon/Turret/BarrelComponent.h"
#include "Weapon/Turret/TurretComponent.h"
#include "Kismet/GameplayStatics.h"

void ATurret::BeginPlay() {
	Super::BeginPlay();
	//ActivateTick(false);
}

void ATurret::Initialize(UBarrelComponent* TankBarrelToSet, UTurretComponent* TankTurretToSet) {
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}

FVector ATurret::SpawnProjectileLocation() {
	if (Barrel == nullptr) return FVector();
	return Barrel->GetSocketLocation(FName("Projectile"));
}


//TODO: aim barrel/turret towards empty space?? dirty fix would be to add walls everywhere
void ATurret::AimAt(FHitResult HitResult, bool bResultHit) {
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FVector EndLocation = HitResult.Location;

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		EndLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
		//FCollisionResponseParams::DefaultResponseParam,
		//TArray<AActor*>(),
		//true //debug draw
	);

	//UE_LOG(LogTemp, Warning, TEXT("line trace end: %d - %d - %d"), HitResult.TraceEnd.X, HitResult.TraceEnd.Y, HitResult.TraceEnd.Z);
	if (bHaveAimSolution) {
		CurrentAimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(CurrentAimDirection);
	}

	FRotator Rotation = Barrel->GetSocketRotation(FName("Projectile"));
	FVector Location = Barrel->GetSocketLocation(FName("Projectile"));
	FVector TransformScale = FVector(1, 1, 1);
	ProjectileSpawnTransform = FTransform(Rotation, Location, TransformScale);
	//else {
	//	MoveBarrelTowards(HitResult.TraceEnd);
	//}
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