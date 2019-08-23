// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "BaseMP_PlayerController.h"

UInteractionComponent::UInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionComponent::Interact() {
	FTransform PawnTransform = GetOwner()->ActorToWorld();
	FVector PawnViewPointLocation = PawnTransform.GetLocation();
	FVector LineTraceEnd = PawnViewPointLocation + PawnTransform.Rotator().Vector() * Reach;

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PawnViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Vehicle),
		TraceParameters
	);

	//TODO:
	// switch / if statement what gets hit???

	// for now swap actors
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		Server_Interact(Controller, ActorHit);
	}
}

//TODO: Get player controller of the client and make that unposses
void UInteractionComponent::Server_Interact_Implementation(APlayerController* Controller, AActor* ActorHit) {
	Controller->UnPossess();
	APawn* pawn = Cast<APawn>(ActorHit);
	if (pawn != nullptr) {
		Controller->Possess(pawn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit on actor is not a pawn"));
	}
}

bool UInteractionComponent::Server_Interact_Validate(APlayerController* Controller, AActor* ActorHit) {
	return true;
}

