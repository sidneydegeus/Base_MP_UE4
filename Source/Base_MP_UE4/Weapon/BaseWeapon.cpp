// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "GenericComponents/PickUpComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ItemUI.h"
#include "GenericComponents/PickUpComponent.h"
#include "Character/BaseCharacter.h"

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWeapon, WeaponData);
}

ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootPlaceholderComponent = CreateDefaultSubobject<USceneComponent>(FName("RootPlaceholderComponent"));
	SetRootComponent(RootPlaceholderComponent);

	PickupComponent = CreateDefaultSubobject<UPickUpComponent>(FName("PickupComponent"));
	PickupComponent->AttachToComponent(RootPlaceholderComponent, FAttachmentTransformRules::KeepRelativeTransform);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(FName("PickupWidget"));
	PickupWidget->AttachToComponent(PickupComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PickupWidget->SetVisibility(false);
	PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	WeaponData.WeaponBlueprint = GetClass();
	if (PickupWidget == nullptr) return;
	UItemUI* ItemUI = Cast<UItemUI>(PickupWidget->GetUserWidgetObject());
	if (ItemUI == nullptr) return;
	ItemUI->SetItemName(GetWeaponName());
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
void ABaseWeapon::AimAt(FHitResult HitResult, bool bResultHit) {
	//INTENDED: override to add functionality
}

void ABaseWeapon::DisablePickUp() {
	Multicast_DisablePickUp();
}

void ABaseWeapon::Multicast_DisablePickUp_Implementation() {
	UPickUpComponent* PickUpComponent = FindComponentByClass<UPickUpComponent>();
	if (PickUpComponent == nullptr) return;
	TArray<USceneComponent*> ChildComponents;
	PickUpComponent->GetChildrenComponents(true, ChildComponents);
	for (USceneComponent* Child : ChildComponents) {
		Child->DestroyComponent();
	}
	PickUpComponent->DestroyComponent();
}


void ABaseWeapon::DisplayWeaponName(bool bDisplay) {
	if (PickupWidget == nullptr) return;
	if (bDisplay) {
		PickupWidget->SetVisibility(true, true);
	}
	else {
		PickupWidget->SetVisibility(false, false);
	}
}

void ABaseWeapon::FindMesh() {
	if (Mesh == nullptr) {
		Mesh = FindComponentByClass<UMeshComponent>();
	}
}



/// Fire 
void ABaseWeapon::Fire() {
	Server_Fire(FTransform());
}

void ABaseWeapon::Server_Fire_Implementation(FTransform Transform) {
	//INTENDED: override to add functionality
}











