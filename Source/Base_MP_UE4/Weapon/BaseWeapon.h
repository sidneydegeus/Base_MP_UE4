// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/BaseProjectile.h"
#include "BaseAimingComponent.h"
#include "BaseWeapon.generated.h"

UCLASS(abstract)
class BASE_MP_UE4_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite)
	UBaseAimingComponent* AimingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ABaseProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Firing")
	uint32 Ammo = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000;

public:	
	ABaseWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UBaseAimingComponent* GetAimingComponent() { return AimingComponent; };

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void Fire();

protected:
	//ABaseProjectile* SpawnProjectile();
	//UFUNCTION()
	//virtual FVector SpawnProjectileLocation();
	//virtual FRotator SpawnProjectileRotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();
	void Server_Fire_Implementation();
	bool Server_Fire_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAmmo(uint32 Amount);
	void Server_SetAmmo_Implementation(uint32 Amount);
	bool Server_SetAmmo_Validate(uint32 Amount);


};
