// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = Animations)
	TMap<EEquipWeaponState, UAnimMontage*> EquipAnimations;

	UPROPERTY(BlueprintReadOnly)
	ABaseCharacter* Character;

	//TODO: maybe change to blueprint read only
	UPROPERTY(BlueprintReadOnly, Category = State)
	EWeaponType WeaponTypeEquipped;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	bool IsSwappingWeapon;

	UPROPERTY(BlueprintReadOnly, Category = State)
	bool IsJumping;



	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float AimPitch;


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//Weapon
	void SetWeaponTypeEquipped(EWeaponType WeaponType) { WeaponTypeEquipped = WeaponType; };
	void SetIsSwappingWeapon(bool Result) { IsSwappingWeapon = Result; };

	void EquipWeaponAnimation(EEquipWeaponState EquipWeaponState);

};

