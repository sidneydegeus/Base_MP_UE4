// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/BaseProjectile.h"
#include "Components/MeshComponent.h"

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWeapon, Ammo);
}

ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ABaseWeapon::Tick(float DeltaTime) {
	//if (GetOwner()->Role == ROLE_Authority) {
	//	if (Ammo <= 0) {
	//		WeaponFiringState = EWeaponFiringState::NoAmmo;
	//	}
	//	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
	//		WeaponFiringState = EWeaponFiringState::Reloading;
	//	}
	//	else {
	//		WeaponFiringState = EWeaponFiringState::Aiming;
	//	}
	//}
}

void ABaseWeapon::ActivateTick(bool bReset) {
	SetActorTickEnabled(bReset);
}

void ABaseWeapon::AimAt(FVector HitLocation) {
	//INTENDED: override to add functionality
}



/// Fire 
void ABaseWeapon::Fire() {
	Server_Fire();
}

void ABaseWeapon::Server_Fire_Implementation() {
	if (!ensure(ProjectileBlueprint)) return;
	ABaseProjectile* Projectile = SpawnProjectile();
	if (Projectile == nullptr) return;
	Projectile->LaunchProjectile(LaunchSpeed);
	Server_SetAmmo(Ammo - 1);
	//TODO: not sure but this should probably be changed for other weapons
	LastFireTime = FPlatformTime::Seconds();
}

bool ABaseWeapon::Server_Fire_Validate() {
	return true;
}


void ABaseWeapon::FindMesh() {
	if (Mesh == nullptr) {
		Mesh = FindComponentByClass<UMeshComponent>();
	}
}

/// Projectile
ABaseProjectile* ABaseWeapon::SpawnProjectile() {
	auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(
		ProjectileBlueprint,
		SpawnProjectileLocation(),
		SpawnProjectileRotation()
		);
	Projectile->SetOwner(this);
	return Projectile;
}

FVector ABaseWeapon::SpawnProjectileLocation() {
	////INTENDED: Override in child for functionality
	//return FVector();
	FindMesh();
	if (Mesh == nullptr) return FVector();
	return Mesh->GetSocketLocation(FName("Muzzle"));
}

FRotator ABaseWeapon::SpawnProjectileRotation() {
	////INTENDED: Override in child for functionality
	//return FRotator();
	FindMesh();
	if (Mesh == nullptr) return FRotator();
	return Mesh->GetSocketRotation(FName("Muzzle"));
}



/// Ammo 
void ABaseWeapon::Server_SetAmmo_Implementation(uint32 Amount) {
	Ammo = Amount;
}

bool ABaseWeapon::Server_SetAmmo_Validate(uint32 Amount) {
	return true;
}

