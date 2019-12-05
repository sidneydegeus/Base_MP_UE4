// Fill out your copyright notice in the Description page of Project Settings.


#include "BeginMeleeAttackNotify.h"
#include "Engine.h"
#include "Character/BaseCharacter.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/Melee/BaseMeleeWeapon.h"

// set  attacking staet teo start

void UBeginMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp == nullptr && MeshComp->GetOwner() == nullptr) return;
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	Character->OnStartAttack();
}