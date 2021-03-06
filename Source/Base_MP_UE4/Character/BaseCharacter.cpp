// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

#include "Weapon/BaseWeapon.h"
#include "Weapon/Unarmed.h"
#include "Weapon/Melee/BaseMeleeWeapon.h"
#include "Weapon/Ranged/BaseRangedWeapon.h"
#include "InteractionComponent.h"
#include "GenericComponents/ExitPawnComponent.h"
#include "GenericComponents/LockOnComponent.h"

#include "UI/PlayerUI.h"
#include "Character/CharacterAnimInstance.h"

#define stringify(name) # name

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, Unarmed);
	DOREPLIFETIME(ABaseCharacter, EquippedWeapon);
	DOREPLIFETIME(ABaseCharacter, MeleeWeaponSlot);
	DOREPLIFETIME(ABaseCharacter, RangedWeaponSlot);

	//UI
	DOREPLIFETIME(ABaseCharacter, CurrentHealth);

	//States for animation
	DOREPLIFETIME(ABaseCharacter, AimPitch);
	DOREPLIFETIME(ABaseCharacter, bJump);
}

ABaseCharacter::ABaseCharacter() {
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Custom"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); // Custom channel Projectile
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	ExitComponent = CreateDefaultSubobject<UExitPawnComponent>(TEXT("ExitableComponent"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input



void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	CurrentHealth = MaxHealth;
	if (HasAuthority()) Server_SetUnarmed();
	if (HasAuthority()) Server_SetDefaultWeapon_Implementation();
}

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsLocallyControlled()) {
		AimPitch = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Pitch + AdditionalAimPitch;
		Server_SetAimPitch(AimPitch);

		if (ManeuverInfo.IsManeuvering) {
			AddMovementInput(ManeuverInfo.ForwardManeuverDirection, ManeuverInfo.ForwardInputValue);
			AddMovementInput(ManeuverInfo.RightManeuverDirection, ManeuverInfo.RightInputValue);
		}
	}
	UpdateUI();
}








/// Movement

void ABaseCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// Turn
void ABaseCharacter::AddControllerYawInput(float Val) {
	if (!LockOnComponent->bIsLockedOnTarget) {
		Super::AddControllerYawInput(Val);
	}
}

void ABaseCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//LookUp
void ABaseCharacter::AddControllerPitchInput(float Val) {
	float Value = UKismetMathLibrary::SelectFloat(0.2f, 1.f, LockOnComponent->bIsLockedOnTarget) * Val;
	Super::AddControllerPitchInput(Value);
}

void ABaseCharacter::MoveForward(float Value) {
	if (Value > 0.0f) {
		MovementInput.bForward = true;
	}
	else if (Value < 0.0f) {
		MovementInput.bBackward = true;
	}
	else {
		MovementInput.bForward = false;
		MovementInput.bBackward = false;
	}

	if (ManeuverInfo.IsManeuvering) return;
	if (Value != 0.0f) {
		AddMovementInput(GetForwardDirection(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value) {
	if (Value > 0.0f) {
		MovementInput.bRight = true;
	}
	else if (Value < 0.0f) {
		MovementInput.bLeft = true;
	}
	else {
		MovementInput.bRight = false;
		MovementInput.bLeft = false;
	}

	if (ManeuverInfo.IsManeuvering) return;
	if (Value != 0.0f) {
		AddMovementInput(GetRightDirection(), Value);
	}
}

void ABaseCharacter::Jump() {
	if (ManeuverInfo.IsManeuvering) return;
	Super::Jump();
	bJump = true;
	if (bIsAttacking) {
		OnStopAttack(true);
	}

}

void ABaseCharacter::StopJumping() {
	Super::StopJumping();
	bJump = false;
	//if (bIsAttacking) {
	//	UE_LOG(LogTemp, Warning, TEXT("on STOP jumping... stop attack???"));
	//	OnStopAttack(true);
	//}
}

void ABaseCharacter::Interact() {
	if (InteractionComponent != nullptr) {
		InteractionComponent->Interact();
	}
}

/// Movement - Maneuver
void ABaseCharacter::Maneuver(float Angle, EManeuverType ManeuverType, float ForwardInputValue, float RightInputValue) {
	if (!GetMovementComponent()->IsFalling() && !ManeuverInfo.IsManeuvering) {
		if (bIsAttacking) {
			OnStopAttack(true);
		}
		ManeuverInfo.ForwardInputValue = ForwardInputValue;
		ManeuverInfo.RightInputValue = RightInputValue;
		ManeuverInfo.ForwardManeuverDirection = GetForwardDirection();
		ManeuverInfo.RightManeuverDirection = GetRightDirection();
		ManeuverInfo.IsManeuvering = true;
		ManeuverInfo.ManeuverAngle = Angle;
		ManeuverInfo.ManeuverType = ManeuverType;
		PreviousMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
		if (ManeuverType == EManeuverType::SideStep) GetCharacterMovement()->MaxWalkSpeed = SideStepSpeed;
		else GetCharacterMovement()->MaxWalkSpeed = DodgeSpeed;
		if (IsLocallyControlled()) {
			Server_Maneuver(Angle, ManeuverType);
			if (bInCombat) {
				GetCharacterMovement()->bOrientRotationToMovement = false;
				bUseControllerRotationYaw = false;
			}
			else {
				DetermineWeaponControlInput(bInCombat);
			}

		}
	}
}

void ABaseCharacter::OnManeuverEnd() {
	ManeuverInfo.IsManeuvering = false;
	ManeuverInfo.ManeuverType = EManeuverType::None;
	GetCharacterMovement()->MaxWalkSpeed = PreviousMaxSpeed;
	if (EquippedWeapon == nullptr) return;
	if (bInCombat) {
		GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon->GetInCombat_CharacterCameraOrientation();
		bUseControllerRotationYaw = EquippedWeapon->GetInCombat_CharacterUseControllerRotationYaw();
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon->GetOutCombat_CharacterCameraOrientation();
		bUseControllerRotationYaw = EquippedWeapon->GetOutCombat_CharacterUseControllerRotationYaw();
	}
}

void ABaseCharacter::Server_Maneuver_Implementation(float Angle, EManeuverType ManeuverType) {
	Multicast_Maneuver(Angle, ManeuverType);
}

void ABaseCharacter::Multicast_Maneuver_Implementation(float Angle, EManeuverType ManeuverType) {
	if (IsLocallyControlled()) return;
	Maneuver(Angle, ManeuverType, 0.f, 0.f);
}

/// Movement - SideStep
void ABaseCharacter::SideStepForward() {
	Maneuver(0.f, EManeuverType::SideStep, 1.f, 0.f);
}

void ABaseCharacter::SideStepBackward() {
	float Angle = -180.f;
	if (bUseControllerRotationYaw == false && GetCharacterMovement()->bOrientRotationToMovement == true) Angle = 0.f;
	Maneuver(Angle, EManeuverType::SideStep, -1.f, 0.f);
}

void ABaseCharacter::SideStepLeft() {
	float Angle = -90.f;
	if (bUseControllerRotationYaw == false && GetCharacterMovement()->bOrientRotationToMovement == true) Angle = 0.f;
	Maneuver(Angle, EManeuverType::SideStep, 0.f, -1.f);
}

void ABaseCharacter::SideStepRight() {
	float Angle = 90.f;
	if (bUseControllerRotationYaw == false && GetCharacterMovement()->bOrientRotationToMovement == true) Angle = 0.f;
	Maneuver(Angle, EManeuverType::SideStep, 0.f, 1.f);
}

/// Movement - Dodge
void ABaseCharacter::Dodge() {
	if (bUseControllerRotationYaw == false && GetCharacterMovement()->bOrientRotationToMovement == true) {
		DodgeFreeCamera();
	}
	else {
		DodgeLockedCamera();
	}
}

void ABaseCharacter::DodgeFreeCamera() {
	if (MovementInput.bForward) {
		if (MovementInput.bLeft) {
			Maneuver(-45.f, EManeuverType::Dodge, 1.f, -1.f);
		}
		else if (MovementInput.bRight) {
			Maneuver(45.f, EManeuverType::Dodge, 1.f, 1.f);
		}
		else {
			Maneuver(0.f, EManeuverType::Dodge, 1.f, 0.f);
		}
	}
	else if (MovementInput.bLeft) {
		if (MovementInput.bBackward) {
			Maneuver(-45.f, EManeuverType::Dodge, -1.f, -1.f);
		}
		else if (MovementInput.bForward) {
			Maneuver(45.f, EManeuverType::Dodge, 1.f, -1.f);
		}
		else {
			Maneuver(0.f, EManeuverType::Dodge, -0.f, -1.f);
		}
	}
	else if (MovementInput.bBackward) {
		if (MovementInput.bLeft) {
			Maneuver(-45.f, EManeuverType::Dodge, -1.f, -1.f);
		}
		else if (MovementInput.bRight) {
			Maneuver(45.f, EManeuverType::Dodge, -1.f, 1.f);
		}
		else {
			Maneuver(0.f, EManeuverType::Dodge, -1.0f, 0.f);
		}
	}
	else if (MovementInput.bRight) {
		if (MovementInput.bForward) {
			Maneuver(-45.f, EManeuverType::Dodge, -1.f, 1.f);
		}
		else if (MovementInput.bBackward) {
			Maneuver(45.f, EManeuverType::Dodge, 1.f, 1.f);
		}
		else {
			Maneuver(0.f, EManeuverType::Dodge, 0.f, 1.f);
		}
	}
}

void ABaseCharacter::DodgeLockedCamera() {
	if (MovementInput.bForward) {
		if (MovementInput.bLeft) { //forward left
			Maneuver(-45.f, EManeuverType::Dodge, 1.f, -1.f);
		}
		else if (MovementInput.bRight) { // forward right
			Maneuver(45.f, EManeuverType::Dodge, 1.f, 1.f);
		}
		else { // forward
			Maneuver(0.f, EManeuverType::Dodge, 1.f, 0.f);
		}
	}
	else if (MovementInput.bBackward) {
		if (MovementInput.bLeft) { //backward left
			Maneuver(-135.f, EManeuverType::Dodge, -1.f, -1.f);
		}
		else if (MovementInput.bRight) { // backward right
			Maneuver(135.f, EManeuverType::Dodge, -1.f, 1.f);
		}
		else { // backward
			Maneuver(-180.f, EManeuverType::Dodge, -1.f, 0.f);
		}
	}
	else if (MovementInput.bLeft) { // left
		Maneuver(-90.f, EManeuverType::Dodge, 0.f, -1.f);
	}
	else if (MovementInput.bRight) { // right
		Maneuver(90.f, EManeuverType::Dodge, 0.f, 1.f);
	}
}






/// Health, Damage, Death and Respawn

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	if (CurrentHealth > 0) {
		Server_SetCurrentHealth(-DamageToApply);
	}
	return DamageToApply;
}

void ABaseCharacter::OnRep_CurrentHealth() {
	float HealthPercentage = (float)CurrentHealth / (float)MaxHealth;
	if (UI == nullptr) return;
	UI->UpdateHealthBar(HealthPercentage);
}

void ABaseCharacter::Server_SetCurrentHealth_Implementation(int32 Value) {
	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0, MaxHealth);
	if (IsLocallyControlled()) OnRep_CurrentHealth();
	if (CurrentHealth <= 0) {
		if (CharacterAnimInstance == nullptr) return;
		// TODO: get an array or something with animation count?
		auto DeathAnimationIndex = FMath::RandRange(1, 3);
		Multicast_OnDeath(DeathAnimationIndex);
	}
}

void ABaseCharacter::OnDeath() {
	bIsAlive = false;
	//GetCapsuleComponent()->SetActive(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player) {
		Player->TargetKilled();
	}
}

void ABaseCharacter::Multicast_OnDeath_Implementation(int32 Index) {
	OnDeath();
	if (!CharacterAnimInstance) return;
	CharacterAnimInstance->SetDeathAnimationIndex(Index);
}

void ABaseCharacter::TargetKilled_Implementation() {
	if (LockOnComponent != nullptr) 
		LockOnComponent->LockOff();
}






/// PickUp Logic
// TODO: eventually change to item and make more generic?
void ABaseCharacter::PickUp() {
	if (OverlappedWeapons.Num() <= 0) return;
	Server_PickUp(OverlappedWeapons[0], this);
}

// TODO: move to an enum .h file
template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "null");
}

void ABaseCharacter::Server_PickUp_Implementation(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner, bool EquipOnPickup) {
	if (WeaponToPickup == nullptr) return;
	FWeaponData Data = WeaponToPickup->GetWeaponData();
	switch (WeaponToPickup->WeaponType) {
		case EWeaponType::Melee:
			MeleeWeaponSlot = SpawnPickedUpWeapon(Data, this, MeleeWeaponSlot);
			if (EquipOnPickup) StartEquipWeapon(DetermineWeaponToArm(MeleeWeaponSlot), false);
			break;
		case EWeaponType::Ranged:
			RangedWeaponSlot = SpawnPickedUpWeapon(Data, this, RangedWeaponSlot);
			if (EquipOnPickup) StartEquipWeapon(DetermineWeaponToArm(RangedWeaponSlot), false);
			break;
	}
	WeaponToPickup->Destroy();
}

//TODO: maybe cleanup function. a bit long and different uses
ABaseWeapon* ABaseCharacter::SpawnPickedUpWeapon(FWeaponData Data, AActor* WeaponOwner, ABaseWeapon* OldWeapon) {
	USceneComponent* CharacterMesh = Cast<USceneComponent>(FindComponentByClass<USkeletalMeshComponent>());
	if (CharacterMesh == nullptr) return nullptr;

	ABaseWeapon* Weapon = GetWorld()->SpawnActor<ABaseWeapon>(Data.WeaponBlueprint, CharacterMesh->GetComponentTransform());	
	if (Weapon == nullptr) return nullptr;
	Weapon->SetOwner(WeaponOwner);
	Weapon->SetWeaponData(Data);
	Weapon->DisablePickUp();

	FString s = EnumToString(stringify(EWeaponType), Weapon->WeaponType) + "WeaponHolsterSocket";
	FName HolsterSocket = FName(*s);
	Weapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, HolsterSocket);

	if (Weapon == nullptr) {
		Weapon = OldWeapon;
	}
	else if (OldWeapon != nullptr) {
		if (OldWeapon->WeaponType == Weapon->WeaponType) {
			OldWeapon->Destroy();
		}
	}
	return Weapon;
}






/// Equip Weapon logic

void ABaseCharacter::WeaponSlot_1() {
	StartEquipWeapon(DetermineWeaponToArm(MeleeWeaponSlot));
}

void ABaseCharacter::WeaponSlot_2() {
	StartEquipWeapon(DetermineWeaponToArm(RangedWeaponSlot));
}

ABaseWeapon* ABaseCharacter::DetermineWeaponToArm(ABaseWeapon* Weapon) {
	ABaseWeapon* WeaponToArm;
	(Weapon == EquippedWeapon) ? WeaponToArm = Unarmed : WeaponToArm = Weapon;
	return WeaponToArm;
}

EEquipWeaponState ABaseCharacter::DetermineEquipWeaponState(ABaseWeapon* Weapon) {
	if (EquippedWeapon == nullptr) return EEquipWeaponState::Unarmed_To_Unarmed;

	if (EquippedWeapon->GetWeaponType() == EWeaponType::Unarmed && Weapon->GetWeaponType() == EWeaponType::Melee) 
		return EEquipWeaponState::Unarmed_To_Melee;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::Unarmed && Weapon->GetWeaponType() == EWeaponType::Ranged)
		return EEquipWeaponState::Unarmed_To_Ranged;

	if (EquippedWeapon->GetWeaponType() == EWeaponType::Melee && Weapon->GetWeaponType() == EWeaponType::Melee)
		return EEquipWeaponState::Melee_To_Melee;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::Melee && Weapon->GetWeaponType() == EWeaponType::Ranged)
		return EEquipWeaponState::Melee_To_Ranged;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::Melee && Weapon->GetWeaponType() == EWeaponType::Unarmed)
		return EEquipWeaponState::Melee_To_Unarmed;

	if (EquippedWeapon->GetWeaponType() == EWeaponType::Ranged && Weapon->GetWeaponType() == EWeaponType::Melee)
		return EEquipWeaponState::Ranged_To_Melee;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::Ranged && Weapon->GetWeaponType() == EWeaponType::Ranged)
		return EEquipWeaponState::Ranged_To_Ranged;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::Ranged && Weapon->GetWeaponType() == EWeaponType::Unarmed)
		return EEquipWeaponState::Ranged_To_Unarmed;

	return EEquipWeaponState::Unarmed_To_Unarmed;
}

void ABaseCharacter::Server_SetUnarmed_Implementation() {
	Unarmed = GetWorld()->SpawnActor<AUnarmed>();
	EquippedWeapon = Unarmed;
	if (IsLocallyControlled()) OnRep_EquippedWeapon();
}

void ABaseCharacter::Server_SetDefaultWeapon_Implementation() {
	if (DefaultWeaponClass) {
		ABaseWeapon* Weapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
		Server_PickUp(Weapon, GetOwner(), true);
	} 
}

void ABaseCharacter::StartEquipWeapon(ABaseWeapon* Weapon, bool UseAnimation) {
	if (Weapon == nullptr) return;

	EEquipWeaponState EquipWeaponState = DetermineEquipWeaponState(Weapon);
	bSwappingWeapon = true;

	if (Role < ROLE_Authority) {
		Server_StartEquipWeapon(Weapon, UseAnimation);
	}
	if (Role == ROLE_Authority) {
		WeaponToUnarm = EquippedWeapon;
		WeaponToEquip = Weapon;
		Multicast_WeaponEquip(EquipWeaponState, UseAnimation);
	}
}

void ABaseCharacter::EquipWeapon() {
	bSwappingWeapon = false;
	if (!HasAuthority()) return;
	USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
	if (CharacterMesh == nullptr || Unarmed == nullptr  || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	FName WeaponSocket = FName(*FString(EnumToString(stringify(EWeaponType), EquippedWeapon->WeaponType) + "WeaponSocket"));
	EquippedWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	DetermineWeaponControlInput(bInCombat);
	if (IsLocallyControlled()) OnRep_EquippedWeapon();
}

void ABaseCharacter::UnequipWeapon() {
	if (!HasAuthority()) return;
	USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
	if (CharacterMesh == nullptr || Unarmed == nullptr || WeaponToUnarm == nullptr) return;
	EquippedWeapon = Unarmed;
	if (WeaponToUnarm != Unarmed) {
		FName HolsterSocket = FName(*FString(EnumToString(stringify(EWeaponType), WeaponToUnarm->WeaponType) + "WeaponHolsterSocket"));
		WeaponToUnarm->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, HolsterSocket);
	}
	DetermineWeaponControlInput(bInCombat);
	if (IsLocallyControlled()) OnRep_EquippedWeapon();
}

void ABaseCharacter::Server_StartEquipWeapon_Implementation(ABaseWeapon* Weapon, bool UseAnimation) {
	StartEquipWeapon(Weapon, UseAnimation);
}

void ABaseCharacter::OnRep_EquippedWeapon() {
	if (CharacterAnimInstance == nullptr || EquippedWeapon == nullptr) return;
	CharacterAnimInstance->SetWeaponTypeEquipped(EquippedWeapon->GetWeaponType());
}

void ABaseCharacter::Multicast_WeaponEquip_Implementation(EEquipWeaponState EquipWeaponState, bool UseAnimation) {
	if (UseAnimation) CharacterAnimInstance->EquipWeaponAnimation(EquipWeaponState);
	else EquipWeapon();
	bIsAttacking = false;
	OnLeaveCombat();
}

void ABaseCharacter::DetermineWeaponControlInput(bool Combat) {
	if (EquippedWeapon == nullptr) return;

	if (Combat) {
		GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon->GetInCombat_CharacterCameraOrientation();
		bUseControllerRotationYaw = EquippedWeapon->GetInCombat_CharacterUseControllerRotationYaw();
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon->GetOutCombat_CharacterCameraOrientation();
		bUseControllerRotationYaw = EquippedWeapon->GetOutCombat_CharacterUseControllerRotationYaw();
	}
}





/// Fire
void ABaseCharacter::Fire() {
	if (EquippedWeapon == nullptr) return;
	if (!bIsAttacking && !ManeuverInfo.IsManeuvering && !bSwappingWeapon) {
		EquippedWeapon->Fire();
		//if (EquippedWeapon->GetWeaponType() == EWeaponType::Melee) {
		//	AttackAnimation();
		//}
	}
}

void ABaseCharacter::SetIsAttacking(bool Value) {
	bIsAttacking = Value;
	if (!bInCombat) EnterCombat();
	ResetCombatTimer();
};

void ABaseCharacter::ResetCombatTimer() {
	GetWorld()->GetTimerManager().SetTimer(LeaveCombatHandle, this, &ABaseCharacter::OnLeaveCombat, LeaveCombatDelay, false);
}

void ABaseCharacter::EnterCombat() {
	bInCombat = true;
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = MaxCombatWalkSpeed;
	if (EquippedWeapon == nullptr) return;
	DetermineWeaponControlInput(bInCombat);
}

////TODO: make generic for all weapon types. Only  works melee now
//void ABaseCharacter::RequestWeaponAnimation() {
//	if (!HasAuthority() || CharacterAnimInstance == nullptr) return;
//
//	if (CharacterAnimInstance->MeleeAttackAnimations.Num() > 0) {
//		auto AttackAnimationIndex = FMath::RandRange(1, CharacterAnimInstance->MeleeAttackAnimations.Num()) - 1;
//		Multicast_MeleeAttack(AttackAnimationIndex);
//	}
//	// TODO: never pick same random number as last random number
//	FTimerDelegate StopAttackDelegate = FTimerDelegate::CreateUObject(this, &ABaseCharacter::OnStopAttack, false);
//	GetWorld()->GetTimerManager().SetTimer(IsAttackingHandle, StopAttackDelegate, ResetAttackDelay, false);
//}

void ABaseCharacter::AttackAnimation() {
	if (Role < ROLE_Authority) {
		Server_AttackAnimation();
	}
	if (Role == ROLE_Authority) {
		if (CharacterAnimInstance != nullptr || CharacterAnimInstance->MeleeAttackAnimations.Num() > 0) {
			auto AttackAnimationIndex = FMath::RandRange(1, CharacterAnimInstance->MeleeAttackAnimations.Num()) - 1;
			//auto Montage = CharacterAnimInstance->MeleeAttackAnimations[AttackAnimationIndex];
			Multicast_AttackAnimation(AttackAnimationIndex);
		}
		FTimerDelegate StopAttackDelegate = FTimerDelegate::CreateUObject(this, &ABaseCharacter::OnStopAttack, false);
		GetWorld()->GetTimerManager().SetTimer(IsAttackingHandle, StopAttackDelegate, ResetAttackDelay, false);
	}
}

void ABaseCharacter::Server_AttackAnimation_Implementation() {
	AttackAnimation();
}

void ABaseCharacter::Multicast_AttackAnimation_Implementation(int32 Index) {
	if (CharacterAnimInstance == nullptr) return;
	CharacterAnimInstance->AttackAnimation(Index);
	//CharacterAnimInstance->EquipWeaponAnimation(EEquipWeaponState::Unarmed_To_Melee);
}






/// Callbacks / Timers

void ABaseCharacter::OnLeaveCombat() {

	GetWorld()->GetTimerManager().ClearTimer(LeaveCombatHandle);
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;
	PreviousMaxSpeed = DefaultMaxWalkSpeed;
	bInCombat = false;
	bIsAttacking = false;
	if (EquippedWeapon == nullptr) return;
	GetCharacterMovement()->bOrientRotationToMovement = EquippedWeapon->GetOutCombat_CharacterCameraOrientation();
	bUseControllerRotationYaw = EquippedWeapon->GetOutCombat_CharacterUseControllerRotationYaw();
}

void ABaseCharacter::OnStartAttack() {
	SetIsAttacking(true);
	if (EquippedWeapon->WeaponType == EWeaponType::Melee) {
		ABaseMeleeWeapon* Weapon = Cast<ABaseMeleeWeapon>(EquippedWeapon);
		Weapon->ResetActorsHit();
		Weapon->ActivateHitBoxCollision();
	}
}

void ABaseCharacter::OnStopAttack(bool bCancelAnimation) {
	GetWorld()->GetTimerManager().ClearTimer(IsAttackingHandle);
	SetIsAttacking(false);
	if (EquippedWeapon->WeaponType == EWeaponType::Melee) {
		ABaseMeleeWeapon* Weapon = Cast<ABaseMeleeWeapon>(EquippedWeapon);
		Weapon->ResetActorsHit();
		Weapon->DeactivateHitBoxCollision();
	}
	if (bCancelAnimation) {
		UE_LOG(LogTemp, Warning, TEXT("stopping animation???"));
		StopAnimMontage();
	}
}




/// Utility

FVector ABaseCharacter::GetForwardDirection() {
	if (Controller != NULL) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector 
		return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	}
	return FVector();
}

FVector ABaseCharacter::GetRightDirection() {
	if (Controller != NULL) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}
	return FVector();
}

float ABaseCharacter::DistanceToCharacter(const ACharacter* OtherCharacter) {
	return GetDistanceTo(OtherCharacter);
}

void ABaseCharacter::UpdateUI() {
	if (UI && GetEquippedWeapon() && GetEquippedWeapon()->WeaponType == EWeaponType::Ranged) {
		ABaseRangedWeapon* RWpn = Cast<ABaseRangedWeapon>(GetEquippedWeapon());
		if (RWpn)  {
			UI->UpdateAmmo(RWpn->GetAmmo());
		}
	}
}