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
	
public:
	ABaseMeleeWeapon();

protected:
	virtual void Server_Fire_Implementation(FTransform Transform) override;
};
