// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAimingComponent.h"
#include "TankAimingComponent.generated.h"


UENUM()
enum class ETankFiringState : uint8
{
	Reloading,
	Aiming,
	NoAmmo
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API UTankAimingComponent : public UBaseAimingComponent
{
	GENERATED_BODY()

//Variables
protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "State")
	ETankFiringState TankFiringState = ETankFiringState::Reloading;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY()
	class UTankBarrel* Barrel = nullptr;
	UPROPERTY()
	class UTankTurret* Turret = nullptr;

	FVector CurrentAimDirection;

//Functions
public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet);

	virtual void AimAt(FVector HitLocation) override;
	virtual void Fire() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual FVector SpawnProjectileLocation() override;
	virtual FRotator SpawnProjectileRotation() override;

private:	
	UTankAimingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveBarrelTowards(FVector CurrentAimDirection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveBarrelTowards(FVector AimDirection);
	void Server_MoveBarrelTowards_Implementation(FVector AimDirection);
	bool Server_MoveBarrelTowards_Validate(FVector AimDirection);

	bool IsBarrelMoving();
};
