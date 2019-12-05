// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "BaseMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ABaseMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<AActor*> ActorsHit;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* HitBoxComponent;

	UPROPERTY(EditDefaultsOnly)
	float MinimumDamage = 15;

	UPROPERTY(EditDefaultsOnly)
	float MaximumDamage = 25;

	
public:
	ABaseMeleeWeapon();
	virtual void BeginPlay() override;
	void ActivateHitBoxCollision();
	void DeactivateHitBoxCollision();
	void ResetActorsHit();

protected:
	virtual void Server_Fire_Implementation(FTransform Transform) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);
};
