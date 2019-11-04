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
	//TODO: maybe change to blueprint read only
	UPROPERTY(BlueprintReadOnly, Category = State)
	EWeaponType WeaponTypeEquipped;

	UPROPERTY(BlueprintReadOnly, Category = State)
	bool IsWeaponEquipped;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	bool IsSwappingWeapon;

	UPROPERTY(BlueprintReadOnly, Category = State)
	bool IsJumping;


public:
	//Weapon
	void SetWeaponTypeEquipped(EWeaponType WeaponType) { WeaponTypeEquipped = WeaponType; };
	void SetWeaponEquipped(bool Result) { IsWeaponEquipped = Result; };
	void SetSwappingWeapon(bool Result) { IsSwappingWeapon = Result; };

	//movement
	void SetIsJumping(bool Result) { IsJumping = Result; };

};

