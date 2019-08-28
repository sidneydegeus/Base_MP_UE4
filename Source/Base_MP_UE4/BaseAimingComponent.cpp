// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAimingComponent.h"

UBaseAimingComponent::UBaseAimingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseAimingComponent::BeginPlay() {
	Super::BeginPlay();	
}

void UBaseAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseAimingComponent::AimAt(FVector HitLocation) {

}

