// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRangedWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/BaseProjectile.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


void ABaseRangedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseRangedWeapon, Ammo);
}

ABaseRangedWeapon::ABaseRangedWeapon() {
	WeaponType = EWeaponType::Ranged;
}


/// Projectile
ABaseProjectile* ABaseRangedWeapon::SpawnProjectile() {
	auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(
		ProjectileBlueprint,
		SpawnProjectileLocation(),
		SpawnProjectileRotation()
		);
	Projectile->SetOwner(this);
	return Projectile;
}

FVector ABaseRangedWeapon::SpawnProjectileLocation() {
	////INTENDED: Override in child for functionality
	//return FVector();
	FindMesh();
	if (Mesh == nullptr) return FVector();
	return Mesh->GetSocketLocation(FName("Muzzle"));
}

FRotator ABaseRangedWeapon::SpawnProjectileRotation() {
	////INTENDED: Override in child for functionality
	//return FRotator();
	FindMesh();
	if (Mesh == nullptr) return FRotator();
	return Mesh->GetSocketRotation(FName("Muzzle"));
}



/// Fire
void ABaseRangedWeapon::Server_Fire_Implementation() {
	if (ProjectileBlueprint == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No projectile Blueprint!!!!"));
		return;
	}
	ABaseProjectile* Projectile = SpawnProjectile();
	if (Projectile == nullptr) return;
	Projectile->LaunchProjectile(FVector::ForwardVector, LaunchSpeed);
	Server_SetAmmo(Ammo - 1);
	//TODO: not sure but this should probably be changed for other weapons
	LastFireTime = FPlatformTime::Seconds();
}



/// Ammo 
void ABaseRangedWeapon::Server_SetAmmo_Implementation(uint32 Amount) {
	Ammo = Amount;
	//Ammo = Amount;
}

bool ABaseRangedWeapon::Server_SetAmmo_Validate(uint32 Amount) {
	return true;
}