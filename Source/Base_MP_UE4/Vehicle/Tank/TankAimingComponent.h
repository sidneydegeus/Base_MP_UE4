// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAimingComponent.h"
#include "TankAimingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API UTankAimingComponent : public UBaseAimingComponent
{
	GENERATED_BODY()

private:
	//UPROPERTY(EditDefaultsOnly, Category = "Firing")
	//	float ReloadTimeInSeconds = 3;

	UPROPERTY()
	class UTankBarrel* Barrel = nullptr;
	UPROPERTY()
	class UTankTurret* Turret = nullptr;

	FVector AimDirection;

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
		void Initialize(UTankBarrel* TankBarrelToSet, UTankTurret* TankTurretToSet);

	virtual void AimAt(FVector HitLocation) override;

protected:
	virtual ABaseProjectile* SpawnProjectile() override;

private:	
	// Called every frame
	UTankAimingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveBarrelTowards(FVector AimDirection);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveBarrelTowards(FVector AimDirection);
		void Server_MoveBarrelTowards_Implementation(FVector AimDirection);
		bool Server_MoveBarrelTowards_Validate(FVector AimDirection);

	bool IsBarrelMoving();
};
