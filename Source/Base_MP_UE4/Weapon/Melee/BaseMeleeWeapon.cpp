// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeWeapon.h"
#include "Character/BaseCharacter.h"

ABaseMeleeWeapon::ABaseMeleeWeapon() {
	WeaponType = EWeaponType::Melee;
}

void ABaseMeleeWeapon::Server_Fire_Implementation(FTransform Transform) {
	// call owner and perform multicast  animation shit
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (BaseCharacter) {
		BaseCharacter->RequestWeaponAnimation();
	}
}