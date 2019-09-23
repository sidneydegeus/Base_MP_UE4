// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnPoint::BeginPlay() {
	Super::BeginPlay();

	////if (GetOwner()->HasAuthority()) {
	//	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	//	if (!SpawnedActor) return;
	//	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
	//	
	//	//// TODO: make into function and server function to set owner
	//	if (GetOwner() != nullptr) {
	//		SpawnedActor->SetOwner(GetOwner());
	//	}
	//	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
	////}
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

