// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Vehicle/BaseVehicleMovementComponent.h"


#include "BaseMP_PlayerController.h"
#include "BaseMP_PlayerState.h"

//TODO: remove
#include "DrawDebugHelpers.h"
#include "StaticLibrary.h"
#include "GameFramework/GameModeBase.h"

ABaseVehicle::ABaseVehicle() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = false;
	NetUpdateFrequency = 30;

	CreateMovementComponent();
	CreateMovementReplicator();
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);
	//PlayerInputComponent->BindAxis("Turn", this, &ABaseVehicle::RotateAzimuthGimbal);
	//PlayerInputComponent->BindAxis("LookUpVehicle", this, &ABaseVehicle::ElevateSpringArm);

	//PlayerInputComponent->BindAction("ExitVehicle", IE_Pressed, this, &ABaseVehicle::ExitPawn);
}

void ABaseVehicle::BeginPlay() {
	Super::BeginPlay();
}

void ABaseVehicle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//FString t1 = "OwnerRole " + UStaticLibrary::GetNetRoleEnumAsString(Role);
	//FString t2 = "RemoteRole " + UStaticLibrary::GetNetRoleEnumAsString(GetRemoteRole());

	//DrawDebugString(GetWorld(), FVector(0, 0, 125), t1, this, FColor::White, DeltaTime);
	//DrawDebugString(GetWorld(), FVector(0, 0, 100), t2, this, FColor::White, DeltaTime);
}


/// Component Creation
void ABaseVehicle::CreateMovementComponent() {
	MovementComponent = CreateDefaultSubobject<UBaseVehicleMovementComponent>(TEXT("MovementComponent"));
}

void ABaseVehicle::CreateMovementReplicator() {
	MovementReplicator = CreateDefaultSubobject<UBaseVehicleMovementReplicator>(TEXT("MovementReplicator"));
}






/// INPUT
void ABaseVehicle::MoveForward(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Throw);
}

void ABaseVehicle::MoveRight(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Throw);
}

/// Possess and UnPossess
void ABaseVehicle::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	Client_PossessedBy(PlayerController);
}

void ABaseVehicle::UnPossessed() {
	Client_UnPossessed();
	SetAutonomousProxy(false);
	Super::UnPossessed();
}




