// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon/BaseWeapon.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(BlueprintReadOnly)
	EWeaponType WeaponTypeEquipped;

	UPROPERTY(BlueprintReadWrite)
	bool bWeaponEquipped;
	
	UPROPERTY(BlueprintReadWrite)
	bool bSwappingWeapon;

public:
	void SetWeaponTypeEquipped(EWeaponType WeaponType) { WeaponTypeEquipped = WeaponType; };
	void SetbWeaponEquipped(bool Result) { bWeaponEquipped = Result; };
	void SetbSwappingWeapon(bool Result) { bSwappingWeapon = Result; };
};
