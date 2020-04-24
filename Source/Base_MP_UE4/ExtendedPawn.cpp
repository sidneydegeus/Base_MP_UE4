// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedPawn.h"

#include "Blueprint/UserWidget.h"
#include "BaseMP_PlayerController.h"
#include "GenericComponents/ExitPawnComponent.h"
#include "Camera/CameraComponent.h"
#include "GenericComponents/SpawnPoint.h"
#include "UI/TankUI.h"
#include "Weapon/Ranged/BaseRangedWeapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

void AExtendedPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AExtendedPawn, Weapon);
}


AExtendedPawn::AExtendedPawn() {
	ExitComponent = CreateDefaultSubobject<UExitPawnComponent>(TEXT("ExitableComponent"));
	CreateCameraComponent();
}

void AExtendedPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateUI();
}

void AExtendedPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AExtendedPawn::RotateAzimuthGimbal);
	PlayerInputComponent->BindAxis("LookUpVehicle", this, &AExtendedPawn::ElevateSpringArm);
	PlayerInputComponent->BindAction("ExitVehicle", IE_Pressed, this, &AExtendedPawn::ExitPawn);
}

void AExtendedPawn::ExitPawn() {
	if (ExitComponent == nullptr) return;
	ExitComponent->ExitPawn();
}

void AExtendedPawn::SetupUIEvent_Implementation() {}
void AExtendedPawn::UpdateUIEvent_Implementation() {}

void AExtendedPawn::Client_PossessedBy_Implementation(APlayerController* PlayerController) {
	if (PlayerController == nullptr) return;
	if (UIClass == nullptr) return;
	UI = CreateWidget<UUserWidget>(PlayerController, UIClass);
	UI->AddToViewport();
	SetupUI();
}

void AExtendedPawn::Client_UnPossessed_Implementation() {
	ABaseMP_PlayerController* PlayerController = Cast<ABaseMP_PlayerController>(GetController());
	if (PlayerController == nullptr) return;
	if (UI == nullptr) return;
	UI->RemoveFromViewport();
}

void AExtendedPawn::RotateAzimuthGimbal(float Delta) {
	FRotator Rotation;
	Rotation.Roll = 0; //x
	Rotation.Pitch = 0; //y
	Rotation.Yaw = Delta; //z
	AzimuthGimbal->AddLocalRotation(Rotation);
}

void AExtendedPawn::ElevateSpringArm(float Delta) {
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

void AExtendedPawn::CreateCameraComponent() {
	USceneComponent* Base = CreateDefaultSubobject<USceneComponent>(FName("Base"));
	SetRootComponent(Base);

	AzimuthGimbal = CreateDefaultSubobject<USceneComponent>(FName("AzimuthGimbal"));
	AzimuthGimbal->SetupAttachment(Base);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(AzimuthGimbal);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
}

ABaseWeapon* AExtendedPawn::GetWeapon() const {
	//auto test = GetComponents();
	//for (UActorComponent* Child : test) {
	//	auto SpawnPointChild = Cast<USpawnPoint>(Child);
	//	if (!SpawnPointChild) continue;

	//	AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
	//	auto Weapon = Cast<ABaseWeapon>(SpawnedChild);
	//	if (Weapon) return Weapon;
	//}
	return nullptr;
}

void AExtendedPawn::SetupUI() {
	SetupUIEvent();
}

void AExtendedPawn::UpdateUI() {
	if (UI && Weapon && Weapon->WeaponType == EWeaponType::Ranged) {
		ABaseRangedWeapon* RWpn = Cast<ABaseRangedWeapon>(GetWeapon());
		if (RWpn) {
			UTankUI* TankUI = Cast<UTankUI>(UI);
			if (TankUI) TankUI->UpdateAmmo(RWpn->GetAmmo());
		}
	}
}