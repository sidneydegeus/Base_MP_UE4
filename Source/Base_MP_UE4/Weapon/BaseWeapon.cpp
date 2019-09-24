// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWeapon, Ammo);
}

ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = false;
	//CreateAimingComponent();
}

void ABaseWeapon::Tick(float DeltaTime) {
	if (GetOwner()->Role == ROLE_Authority) {
		if (Ammo <= 0) {
			WeaponFiringState = EWeaponFiringState::NoAmmo;
		}
		else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
			WeaponFiringState = EWeaponFiringState::Reloading;
		}
		else {
			WeaponFiringState = EWeaponFiringState::Aiming;
		}
	}
}

void ABaseWeapon::Fire() {
	Server_Fire();
}

void ABaseWeapon::Server_Fire_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("im firing"));
	//if (!ensure(ProjectileBlueprint)) return;
	//ABaseProjectile* Projectile = SpawnProjectile();
	//if (Projectile == nullptr) return;
	//Projectile->LaunchProjectile(LaunchSpeed);
	//Server_SetAmmo(Ammo - 1);
	//LastFireTime = FPlatformTime::Seconds();
}

bool ABaseWeapon::Server_Fire_Validate() {
	return true;
}

void ABaseWeapon::Server_SetAmmo_Implementation(uint32 Amount) {
	Ammo = Amount;
}

bool ABaseWeapon::Server_SetAmmo_Validate(uint32 Amount) {
	return true;
}

