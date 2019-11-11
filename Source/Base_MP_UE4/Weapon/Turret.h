// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Ranged/BaseRangedWeapon.h"
#include "Turret.generated.h"

// TODO: 
UCLASS()
class BASE_MP_UE4_API ATurret : public ABaseRangedWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class UTurretComponent* Turret = nullptr;
	UPROPERTY()
	class UBarrelComponent* Barrel = nullptr;

	FVector CurrentAimDirection;

public:
	virtual void AimAt(FHitResult HitResult, bool bHitResult) override;

protected:
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UBarrelComponent* TankBarrelToSet, UTurretComponent* TankTurretToSet);

	virtual FVector SpawnProjectileLocation() override;


private:
	void MoveBarrelTowards(FVector CurrentAimDirection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveBarrelTowards(FVector AimDirection);
	void Server_MoveBarrelTowards_Implementation(FVector AimDirection);
	bool Server_MoveBarrelTowards_Validate(FVector AimDirection);

	bool IsBarrelMoving();
};
