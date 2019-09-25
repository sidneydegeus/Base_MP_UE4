// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "Turret.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ATurret : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class UTankBarrel* Barrel = nullptr;
	UPROPERTY()
	class UTankTurret* Turret = nullptr;

	FVector CurrentAimDirection;

public:
	virtual void AimAt(FVector HitLocation) override;

protected:
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet);

private:
	void MoveBarrelTowards(FVector CurrentAimDirection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveBarrelTowards(FVector AimDirection);
	void Server_MoveBarrelTowards_Implementation(FVector AimDirection);
	bool Server_MoveBarrelTowards_Validate(FVector AimDirection);

	bool IsBarrelMoving();
};
