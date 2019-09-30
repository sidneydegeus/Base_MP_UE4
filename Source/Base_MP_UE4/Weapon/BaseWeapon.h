// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/BaseProjectile.h"
#include "BaseWeapon.generated.h"

UENUM()
enum class EWeaponFiringState : uint8
{
	Reloading,
	Aiming,
	NoAmmo
};

UCLASS(abstract)
class BASE_MP_UE4_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName HolsterSocket;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ABaseProjectile> ProjectileBlueprint;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "State")
	EWeaponFiringState WeaponFiringState = EWeaponFiringState::Reloading;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Firing")
	uint32 Ammo = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	UPROPERTY()
	class UMeshComponent* Mesh;

public:	
	ABaseWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void Tick(float DeltaTime) override;

	void ActivateTick(bool bReset);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void Fire();

	virtual void AimAt(FVector HitLocation);

protected:
	void FindMesh();
	ABaseProjectile* SpawnProjectile();
	virtual FVector SpawnProjectileLocation();
	virtual FRotator SpawnProjectileRotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();
	void Server_Fire_Implementation();
	bool Server_Fire_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAmmo(uint32 Amount);
	void Server_SetAmmo_Implementation(uint32 Amount);
	bool Server_SetAmmo_Validate(uint32 Amount);
};
