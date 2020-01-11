// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character == nullptr) return;

	IsJumping = Character->bJump;
	AimPitch = Character->AimPitch;
	IsAlive = Character->bIsAlive;
	InCombat = Character->GetInCombat();
	ManeuverInfo = Character->ManeuverInfo;

	if (Character->GetEquippedWeapon() == nullptr) return;
	SetWeaponTypeEquipped(Character->GetEquippedWeapon()->GetWeaponType());
}

void UCharacterAnimInstance::EquipWeaponAnimation(EEquipWeaponState EquipWeaponState) {
	if (EquipWeaponState == EEquipWeaponState::Unarmed_To_Unarmed) return;
	Montage_Play(EquipAnimations[EquipWeaponState]);
}