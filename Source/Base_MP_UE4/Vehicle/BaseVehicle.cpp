// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

#include "Components/InputComponent.h"
#include "Vehicle/BaseVehicleMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseVehicle::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseVehicle::RotateAzimuthGimbal);
	PlayerInputComponent->BindAxis("LookUpVehicle", this, &ABaseVehicle::ElevateSpringArm);

	//TODO: remove
	//PlayerInputComponent->BindAction("Interact", this, 
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
	Rotation.Pitch = Delta; //y
	SpringArm->AddLocalRotation(Rotation);
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

void ABaseVehicle::ChangeVehicle() {

}

