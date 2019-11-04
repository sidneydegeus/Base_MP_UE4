// Fill out your copyright notice in the Description page of Project Settings.


#include "Unarmed.h"

AUnarmed::AUnarmed() {
	WeaponType = EWeaponType::Unarmed;
	WeaponName = "Unarmed";
	bReplicates = true;

	//Retarded code, but an actor needs a root component...
	Mesh = Cast<UMeshComponent>(CreateDefaultSubobject<USceneComponent>("Mesh"));
	SetRootComponent(Cast<USceneComponent>(Mesh));
}