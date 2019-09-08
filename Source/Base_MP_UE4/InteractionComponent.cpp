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

// TODO: Cleanup class and make it tidy
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

	// for now swap actors
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		//if (PlayerController == nullptr) return;
		//ABaseVehicle* Vehicle = Cast<ABaseVehicle>(ActorHit);
		//if (Vehicle == nullptr) return;
		//Vehicle->EnterVehicle(PlayerController);
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

//TODO: Get player controller of the client and make that unposses
void UInteractionComponent::Server_Interact_Implementation(APlayerController* Controller, APawn* Pawn) {

	//APawn* OldPawn = Controller->GetPawn();
	//if (OldPawn == nullptr) return;
	//ABaseMP_PlayerState* State = Controller->GetPlayerState<ABaseMP_PlayerState>();
	//State->SetMainCharacter(OldPawn);

	Controller->Possess(Pawn);
}

bool UInteractionComponent::Server_Interact_Validate(APlayerController* Controller, APawn* Pawn) {
	return true;
}

