// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#include "BaseMP_PlayerController.h"
#include "BaseMP_PlayerState.h"

UInteractionComponent::UInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// scan
	//LineTrace();
	//IsInteractable();
}

void UInteractionComponent::LineTrace() {

}

void UInteractionComponent::IsInteractable() {
	if (ActorHit) {
		bInteract = true;
	}
	else {
		bInteract = false;
	}
}

void UInteractionComponent::SetActorHit(AActor* NewActorHit) {
	if (ActorHit != NewActorHit) {
		ActorHit = NewActorHit;
		//call blueprintevent?
	}
}

// TODO: Identify the actorhit and what object it really is. e.g. vehicle, item etc
void UInteractionComponent::Interact() {
	FTransform PawnTransform = GetOwner()->ActorToWorld();
	FVector PawnViewPointLocation = PawnTransform.GetLocation();
	FVector LineTraceEnd = PawnViewPointLocation + PawnTransform.Rotator().Vector() * Reach;

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FCollisionObjectQueryParams ObjQueryParams;
	ObjQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	//ObjQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PawnViewPointLocation,
		LineTraceEnd,
		ObjQueryParams,
		TraceParameters
	);
	DrawDebugLine(GetWorld(), PawnViewPointLocation, LineTraceEnd, FColor::Green, false, 1, 0, 1); //
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		if (Controller == nullptr) return;
		APawn* Pawn = Cast<APawn>(ActorHit);
		if (Pawn == nullptr) return;
		Server_Interact(Controller, Pawn);
		if (GetOwnerRole() == ROLE_Authority) {
			Pawn->SetAutonomousProxy(false);
		}
	}
}

void UInteractionComponent::Server_Interact_Implementation(APlayerController* Controller, APawn* Pawn) {
	Controller->Possess(Pawn);
}

bool UInteractionComponent::Server_Interact_Validate(APlayerController* Controller, APawn* Pawn) {
	return true;
}

