// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "BaseWeaponCharacter.generated.h"


UCLASS(abstract)
class BASE_MP_UE4_API ABaseWeaponCharacter : public ABaseWeapon
{
	GENERATED_BODY()

public:


protected:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PickUpRadius;

public:
	ABaseWeaponCharacter();
	void DisablePickUpRadius();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisablePickUpRadius();
	void Multicast_DisablePickUpRadius_Implementation();
};
