// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedPawn.h"
#include "TurretBase.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ATurretBase : public AExtendedPawn
{
	GENERATED_BODY()
	



	//Functions
public:
	ATurretBase();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealthPercentage() const;

	//FTankDelegate OnDeath;

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

};
