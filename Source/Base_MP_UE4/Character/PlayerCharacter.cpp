// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UI/PlayerUI.h"
#include "BaseMP_PlayerController.h"

#include "TimerManager.h"
#include "GenericComponents/ExitPawnComponent.h"
#include "GenericComponents/LockOnComponent.h"

APlayerCharacter::APlayerCharacter() {

}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("SideStep_Forward", IE_DoubleClick, this, &APlayerCharacter::SideStepForward);
	PlayerInputComponent->BindAction("SideStep_Backward", IE_DoubleClick, this, &APlayerCharacter::SideStepBackward);
	PlayerInputComponent->BindAction("SideStep_Left", IE_DoubleClick, this, &APlayerCharacter::SideStepLeft);
	PlayerInputComponent->BindAction("SideStep_Right", IE_DoubleClick, this, &APlayerCharacter::SideStepRight);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::Dodge);
	//PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &ABaseCharacter::Dodge);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);

	PlayerInputComponent->BindAction("Weapon 1", IE_Pressed, this, &APlayerCharacter::WeaponSlot_1);
	PlayerInputComponent->BindAction("Weapon 2", IE_Pressed, this, &APlayerCharacter::WeaponSlot_2);

	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &APlayerCharacter::PickUp);
	PlayerInputComponent->BindAction("LockTarget", IE_Pressed, this, &APlayerCharacter::LockTarget);

	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &APlayerCharacter::Test);	
}

void APlayerCharacter::Test() {
	UE_LOG(LogTemp, Warning, TEXT("array size = %d"), OverlappedWeapons.Num());
}

void APlayerCharacter::OnRep_CurrentHealth() {
	float HealthPercentage = (float)CurrentHealth / (float)MaxHealth;
	if (UI == nullptr) return;
	UI->UpdateHealthBar(HealthPercentage);
}

void APlayerCharacter::OnDeath() {
	Super::OnDeath();
	if (IsLocallyControlled() && PlayerController) {
		DisableInput(PlayerController);
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &APlayerCharacter::OnRespawn, DestroyCharacterDeathDelay, false);
	}
}

void APlayerCharacter::OnRespawn() {
	if (!ExitComponent) return;
	ExitComponent->ExitPawn();
	Destroy();
}





void APlayerCharacter::OnRep_EquippedWeapon() {
	Super::OnRep_EquippedWeapon();
	if (IsLocallyControlled() && PlayerController && UI) {
		DetermineWeaponControlInput(bInCombat);
		PlayerController->SetWeapon(EquippedWeapon);
		UI->SetWeaponNameText(EquippedWeapon->GetWeaponName());
		UI->DisplayCrosshair(EquippedWeapon->GetWeaponType() == EWeaponType::Ranged ? true : false);
	}
}


void APlayerCharacter::LockTarget() {
	if (LockOnComponent) LockOnComponent->LockOn();
}


/// Possession
void APlayerCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	ABaseMP_PlayerController* NewPlayerController = Cast<ABaseMP_PlayerController>(GetController());
	Client_PossessedBy(NewPlayerController);
}

void APlayerCharacter::UnPossessed() {
	Super::UnPossessed();
	SetAutonomousProxy(false);
	Destroy();
	Client_UnPossessed();
}

void APlayerCharacter::Client_PossessedBy_Implementation(ABaseMP_PlayerController* NewPlayerController) {
	if (NewPlayerController == nullptr) return;
	PlayerController = NewPlayerController;
	if (UIClass == nullptr) return;
	UI = CreateWidget<UPlayerUI>(PlayerController, UIClass);
	UI->AddToViewport();
}

void APlayerCharacter::Client_UnPossessed_Implementation() {
	if (PlayerController == nullptr) return;
	if (UI == nullptr) return;
	UI->RemoveFromViewport();
}