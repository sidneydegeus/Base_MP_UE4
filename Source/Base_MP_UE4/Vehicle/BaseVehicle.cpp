// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "BaseMP_PlayerController.h"
#include "BaseMP_PlayerState.h"

//TODO: remove
#include "DrawDebugHelpers.h"
#include "StaticLibrary.h"

ABaseVehicle::ABaseVehicle() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = false;
	NetUpdateFrequency = 30;

	CreateCameraComponent();
	CreateMovementComponent();
	CreateMovementReplicator();
}

void ABaseVehicle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FString t1 = "OwnerRole " + UStaticLibrary::GetNetRoleEnumAsString(Role);
	FString t2 = "RemoteRole " + UStaticLibrary::GetNetRoleEnumAsString(GetRemoteRole());

	DrawDebugString(GetWorld(), FVector(0, 0, 125), t1, this, FColor::White, DeltaTime);
	DrawDebugString(GetWorld(), FVector(0, 0, 100), t2, this, FColor::White, DeltaTime);
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseVehicle::RotateAzimuthGimbal);
	PlayerInputComponent->BindAxis("LookUpVehicle", this, &ABaseVehicle::ElevateSpringArm);

	PlayerInputComponent->BindAction("ExitVehicle", IE_Pressed, this, &ABaseVehicle::ExitVehicle);
}

void ABaseVehicle::BeginPlay() {
	Super::BeginPlay();
}

void ABaseVehicle::CreateMovementComponent() {
	MovementComponent = CreateDefaultSubobject<UBaseVehicleMovementComponent>(TEXT("MovementComponent"));
}

void ABaseVehicle::CreateMovementReplicator() {
	MovementReplicator = CreateDefaultSubobject<UBaseVehicleMovementReplicator>(TEXT("MovementReplicator"));
}

void ABaseVehicle::MoveForward(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Throw);
}

void ABaseVehicle::MoveRight(float Throw) {
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Throw);
}

void ABaseVehicle::RotateAzimuthGimbal(float Delta) {
	FRotator Rotation;
	Rotation.Roll = 0; //x
	Rotation.Pitch = 0; //y
	Rotation.Yaw = Delta; //z
	AzimuthGimbal->AddLocalRotation(Rotation);
}

void ABaseVehicle::ElevateSpringArm(float Delta) {
	float Min = -70;
	float Max = 20;
	TotalDeltaPitch += Delta;
	TotalDeltaPitch = FMath::Clamp(TotalDeltaPitch, Min, Max);
	if (TotalDeltaPitch >= Max || TotalDeltaPitch <= Min) Delta = 0;

	FRotator Rotation;
	Rotation.Roll = 0; //x
	Rotation.Pitch = Delta; //y
	Rotation.Yaw = 0; // z
	SpringArm->AddLocalRotation(Rotation);
}

void ABaseVehicle::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	if (Role == ROLE_Authority) 
		SetAutonomousProxy(false);
}

void ABaseVehicle::UnPossessed() {
	Super::UnPossessed();
	SetAutonomousProxy(false);
}


void ABaseVehicle::CreateCameraComponent() {
	USceneComponent* Base = CreateDefaultSubobject<USceneComponent>(FName("Base"));
	SetRootComponent(Base);

	AzimuthGimbal = CreateDefaultSubobject<USceneComponent>(FName("AzimuthGimbal"));
	AzimuthGimbal->SetupAttachment(Base);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(AzimuthGimbal);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABaseVehicle::ExitVehicle() {

	/*APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	Server_ExitVehicle_Implementation(PlayerController);*/
}

void ABaseVehicle::Server_ExitVehicle_Implementation(APlayerController* PlayerController) {
	if (Controller == nullptr) return;
	ABaseMP_PlayerState* State = Controller->GetPlayerState<ABaseMP_PlayerState>();
	if (State == nullptr) return;
	APawn* MainCharacter = State->GetMainCharacter();
	if (MainCharacter == nullptr) return; 
	Controller->UnPossess();
	UE_LOG(LogTemp, Warning, TEXT("main character %s"), *MainCharacter->GetName());
	Controller->Possess(MainCharacter);
}

bool ABaseVehicle::Server_ExitVehicle_Validate(APlayerController* PlayerController) {
	return true;
}

