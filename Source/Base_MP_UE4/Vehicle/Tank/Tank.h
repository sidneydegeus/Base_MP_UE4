// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicle/BaseVehicle.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ATank : public ABaseVehicle
{
	GENERATED_BODY()
	
//Variables
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 MaxHealth = 100;

	int32 CurrentHealth;

	//UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)
	//	FTransform ReplicatedTransform;

	//Functions
public:
	ATank();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	FTankDelegate OnDeath;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

private:
	UFUNCTION(BlueprintCallable)
	class ABaseWeapon* GetWeapon() const;

	void Fire();
	//UFUNCTION()
	//	void OnRep_ReplicatedTransform();
};
