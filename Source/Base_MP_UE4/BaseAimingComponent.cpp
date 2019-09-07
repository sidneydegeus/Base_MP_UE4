// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAimingComponent.h"
#include "Net/UnrealNetwork.h"

void UBaseAimingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseAimingComponent, Ammo);
}

UBaseAimingComponent::UBaseAimingComponent() {
	// Child needs to put tick specifically itself
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = false;
	//PrimaryComponentTick.
	//SetComponentTickEnabled(true);
	//bAutoActivate = false;
	//SetIsReplicated(true);

}

void UBaseAimingComponent::BeginPlay() {
	Super::BeginPlay();	
}

void UBaseAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseAimingComponent::Activate(bool bReset) {
	Super::Activate(bReset);
	//PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);
}

void UBaseAimingComponent::Deactivate() {
	Super::Deactivate();
	SetComponentTickEnabled(false);
	//PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAimingComponent::AimAt(FVector HitLocation) {
	// Child classes need to implement this
}

void UBaseAimingComponent::Fire() {
	Server_Fire();
}

ABaseProjectile* UBaseAimingComponent::SpawnProjectile() {
	return nullptr;
}

void UBaseAimingComponent::Server_Fire_Implementation() {
	if (!ensure(ProjectileBlueprint)) return;
	ABaseProjectile* Projectile = SpawnProjectile();
	if (Projectile == nullptr) return;
	Projectile->LaunchProjectile(LaunchSpeed);
	Server_SetAmmo(Ammo - 1);
	LastFireTime = FPlatformTime::Seconds();
}

bool UBaseAimingComponent::Server_Fire_Validate() {
	return true;
}

void UBaseAimingComponent::Server_SetAmmo_Implementation(uint32 Amount) {
	Ammo = Amount;
}

bool UBaseAimingComponent::Server_SetAmmo_Validate(uint32 Amount) {
	return true;
}

