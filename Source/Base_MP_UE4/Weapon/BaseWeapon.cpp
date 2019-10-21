// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Components/MeshComponent.h"
#include "GenericComponents/PickUpComponent.h"

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWeapon, WeaponData);
}

ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	WeaponData.WeaponBlueprint = GetClass();
}

void ABaseWeapon::Tick(float DeltaTime) {
	//if (GetOwner()->Role == ROLE_Authority) {
	//	if (Ammo <= 0) {
	//		WeaponFiringState = EWeaponFiringState::NoAmmo;
	//	}
	//	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
	//		WeaponFiringState = EWeaponFiringState::Reloading;
	//	}
	//	else {
	//		WeaponFiringState = EWeaponFiringState::Aiming;
	//	}
	//}
}

void ABaseWeapon::ActivateTick(bool bReset) {
	SetActorTickEnabled(bReset);
}

//TODO: maybe move to BaseRangedWeapon instead
void ABaseWeapon::AimAt(FVector HitLocation) {
	//INTENDED: override to add functionality
}

void ABaseWeapon::DisablePickUp() {
	Multicast_DisablePickUp();
}

void ABaseWeapon::Multicast_DisablePickUp_Implementation() {
	UPickUpComponent* PickUpComponent = FindComponentByClass<UPickUpComponent>();
	if (PickUpComponent == nullptr) return;
	TArray<USceneComponent*> Children;
	PickUpComponent->GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children) {
		Child->DestroyComponent();
	}
	PickUpComponent->DestroyComponent();
}


void ABaseWeapon::DisplayWeaponName_Implementation(bool bDisplay) {}

void ABaseWeapon::FindMesh() {
	if (Mesh == nullptr) {
		Mesh = FindComponentByClass<UMeshComponent>();
	}
}



/// Fire 
void ABaseWeapon::Fire() {
	Server_Fire();
}

void ABaseWeapon::Server_Fire_Implementation() {
	//INTENDED: override to add functionality
}

bool ABaseWeapon::Server_Fire_Validate() {
	return true;
}










