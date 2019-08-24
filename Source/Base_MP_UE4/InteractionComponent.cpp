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
}

void UInteractionComponent::Interact() {
	FTransform PawnTransform = GetOwner()->ActorToWorld();
	FVector PawnViewPointLocation = PawnTransform.GetLocation();
	FVector LineTraceEnd = PawnViewPointLocation + PawnTransform.Rotator().Vector() * Reach;

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FCollisionObjectQueryParams ObjQueryParams;
	ObjQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	ObjQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PawnViewPointLocation,
		LineTraceEnd,
		ObjQueryParams,
		TraceParameters
	);

	// for now swap actors
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		Server_Interact(Controller, ActorHit);
	}
}

//TODO: Get player controller of the client and make that unposses
void UInteractionComponent::Server_Interact_Implementation(APlayerController* Controller, AActor* ActorHit) {
	if (Controller == nullptr) return;
	APawn* OldPawn = Controller->GetPawn();
	if (OldPawn == nullptr) return;
	ABaseMP_PlayerState* State = Controller->GetPlayerState<ABaseMP_PlayerState>();
	State->SetMainCharacter(OldPawn);
	APawn* Pawn = Cast<APawn>(ActorHit);
	if (Pawn == nullptr) return;
	Controller->UnPossess();
	Controller->Possess(Pawn);
}

bool UInteractionComponent::Server_Interact_Validate(APlayerController* Controller, AActor* ActorHit) {
	return true;
}

