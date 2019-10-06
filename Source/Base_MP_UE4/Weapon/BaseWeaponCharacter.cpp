// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

ABaseWeaponCharacter::ABaseWeaponCharacter() {
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	PickUpRadius = CreateDefaultSubobject<USphereComponent>("PickUpRadius");
	PickUpRadius->SetupAttachment(WeaponMesh);
	PickUpRadius->SetCollisionProfileName(FName("Custom"));
	PickUpRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickUpRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	PickUpRadius->SetSphereRadius(64);
}

void ABaseWeaponCharacter::BeginPlay() {
	Super::BeginPlay();
	WeaponData.WeaponBlueprint = GetClass();
}

void ABaseWeaponCharacter::DisablePickUpRadius() {
	Multicast_DisablePickUpRadius();
}

void ABaseWeaponCharacter::Multicast_DisablePickUpRadius_Implementation() {
	PickUpRadius->DestroyComponent();
}