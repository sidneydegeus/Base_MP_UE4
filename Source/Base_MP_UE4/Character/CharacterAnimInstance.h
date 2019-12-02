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
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Animations)
	TArray<UAnimMontage*> MeleeAttackAnimations;

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

	UPROPERTY(BlueprintReadOnly, Category = State)
	bool InCombat;

	UPROPERTY(BlueprintReadOnly, Category = State)
	FSideStepInfo SideStep;

	UPROPERTY(BlueprintReadOnly, Category = State)
	ECharacterHealthState CharacterHealthState = ECharacterHealthState::Alive;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float AimPitch;

	UPROPERTY(BlueprintReadOnly)
	int32 DeathAnimationIndex;


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetDeathAnimationIndex(int32 Index) { DeathAnimationIndex = Index; };

	//Weapon
	void SetWeaponTypeEquipped(EWeaponType WeaponType) { WeaponTypeEquipped = WeaponType; };
	void SetIsSwappingWeapon(bool Result) { IsSwappingWeapon = Result; };

	void EquipWeaponAnimation(EEquipWeaponState EquipWeaponState);
};

