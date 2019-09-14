// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Projectile/BaseProjectile.h"
#include "BaseAimingComponent.generated.h"


UCLASS(abstract)
class BASE_MP_UE4_API UBaseAimingComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Firing")
		uint32 Ammo = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<ABaseProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 4000;

	double LastFireTime = 0;

// Functions
public:	
	UBaseAimingComponent();
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	virtual void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
		virtual void Fire();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

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
