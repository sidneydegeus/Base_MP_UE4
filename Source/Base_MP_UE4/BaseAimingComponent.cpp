// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAimingComponent.h"
#include "Net/UnrealNetwork.h"

void UBaseAimingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBaseAimingComponent, Ammo);
}

UBaseAimingComponent::UBaseAimingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UBaseAimingComponent::BeginPlay() {
	Super::BeginPlay();	
}

void UBaseAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseAimingComponent::AimAt(FVector HitLocation) {

}

