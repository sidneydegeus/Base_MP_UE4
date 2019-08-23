// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "BaseMP_PlayerController.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Interact();
	//FVector PlayerViewPointLocation;
	//FRotator PlayerViewPointRotation;
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	//	OUT PlayerViewPointLocation,
	//	OUT PlayerViewPointRotation
	//);

	//FVector PawnViewPointLocation;

	//FTransform test = GetOwner()->ActorToWorld();
	//PawnViewPointLocation = test.GetLocation();
	//PawnViewPointRotation = test.GetRotation().GetRotationAxis();

	//FVector LineTraceEnd = PawnViewPointLocation + test.Rotator().Vector() * Reach;
	/// Draw a red trace in the world to visualise
	/*DrawDebugLine(
		GetWorld(),
		PawnViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		50.f*/
	//);

	///// Setup query parameters
	//FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///// Line-trace (AKA ray-cast) out to reach distance
	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(
	//	OUT Hit,
	//	PawnViewPointLocation,
	//	LineTraceEnd,
	//	FCollisionObjectQueryParams(ECollisionChannel::ECC_Vehicle),
	//	TraceParameters
	//);

	/// See what what we hit
	//AActor* ActorHit = Hit.GetActor();
	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	//}
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
		Controller->UnPossess();
		APawn* pawn = Cast< APawn>(ActorHit);
		if (pawn != nullptr) {
			Controller->Possess(pawn);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit on actor is not a pawn"));
		}
	}
}

