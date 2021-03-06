// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

UENUM()
enum class EWeaponFiringState : uint8
{
	Reloading,
	Aiming,
	NoAmmo
};

UCLASS(abstract)
class BASE_MP_UE4_API ABaseRangedWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<ABaseProjectile> ProjectileBlueprint;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "State")
		EWeaponFiringState WeaponFiringState = EWeaponFiringState::Reloading;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Firing")
	int Ammo = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 4000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	FTransform ProjectileSpawnTransform;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	ABaseRangedWeapon();
	void AimAt(FHitResult HitResult, bool bResultHit) override;
	virtual void Fire() override;
	int GetAmmo() { return Ammo; };

protected:
	bool SpawnProjectile(FTransform Transform);

	virtual FVector SpawnProjectileLocation();

	virtual void Server_Fire_Implementation(FTransform Transform) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAmmo(uint32 Amount);
	void Server_SetAmmo_Implementation(uint32 Amount);
	bool Server_SetAmmo_Validate(uint32 Amount) { return true; };

};
