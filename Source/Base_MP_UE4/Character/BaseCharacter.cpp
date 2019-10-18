// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Weapon/BaseWeapon.h"
#include "InteractionComponent.h"

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, EquippedWeapon);
	DOREPLIFETIME(ABaseCharacter, WeaponSlots);
}

ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::Interact);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::Fire);

	PlayerInputComponent->BindAction("ActionBar 1", IE_Pressed, this, &ABaseCharacter::ActionBar1);
	PlayerInputComponent->BindAction("ActionBar 2", IE_Pressed, this, &ABaseCharacter::ActionBar2);
}

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


/// Movement

void ABaseCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::MoveForward(float Value) {
	if ((Controller != NULL) && (Value != 0.0f)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value) {
	if ((Controller != NULL) && (Value != 0.0f)) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Interact() {
	if (InteractionComponent != nullptr) {
		InteractionComponent->Interact();
	}
}




/// PickUp Logic
// TODO: eventually change to item and make more generic?
void ABaseCharacter::PickUp() {
	if (OverlappedWeapon == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("no overlapping weapon"));
		return;
	}
	Server_PickUp(OverlappedWeapon, this);
	OverlappedWeapon = nullptr;
}

// TODO: Cleanup code. For loop used over and over again. Code redundancy
void ABaseCharacter::Server_PickUp_Implementation(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner) {
	if (WeaponToPickup == nullptr) return;
	FWeaponData Data = WeaponToPickup->GetWeaponData();

	if (!FillEmptyWeaponSlot(Data)) {
		for (FWeaponSlot& Slot : WeaponSlots) {
			if (EquippedWeapon == nullptr) {
				ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
				Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
				OldHolsteredWeapon->Destroy();
				// TODO: spawn this old holstered weapon on the floor with old data
				break;
			}
			else {
				if (Slot.HolsterSocket == EquippedWeapon->GetWeaponData().HolsterSocket) {
					ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
					Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
					OldHolsteredWeapon->Destroy();
					EquipWeapon(Slot.Weapon);
					// TODO: spawn this old holstered weapon on the floor with old data
					break;
				}
			}
		}
		//if (EquippedWeapon == nullptr) {	
		//	// swap first slot weapon with picked up weapon
		//	for (FWeaponSlot& Slot : WeaponSlots) {
		//		ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
		//		Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
		//		OldHolsteredWeapon->Destroy();
		//		// TODO: spawn this old holstered weapon on the floor with old data
		//		break;
		//	}
		//}
		//else {
		//	for (FWeaponSlot& Slot : WeaponSlots) {
		//		if (Slot.HolsterSocket == EquippedWeapon->GetWeaponData().HolsterSocket) {
		//			ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
		//			Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
		//			OldHolsteredWeapon->Destroy();
		//			EquipWeapon(Slot.Weapon);
		//			// TODO: spawn this old holstered weapon on the floor with old data
		//			break;
		//		}
		//	}
		//}
	}

	//if (EquippedWeapon == nullptr) {
	//	if (!FillEmptyWeaponSlot(Data)) {
	//		// swap first slot weapon with picked up weapon
	//		for (FWeaponSlot& Slot : WeaponSlots) {
	//			ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
	//			Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
	//			OldHolsteredWeapon->Destroy();
	//			// TODO: spawn this old holstered weapon on the floor with old data
	//			break;		
	//		}
	//	}
	//}
	//else {
	//	if (!FillEmptyWeaponSlot(Data)) {
	//		// swap equipped weapon with picked up weapon
	//		for (FWeaponSlot& Slot : WeaponSlots) {
	//			if (Slot.HolsterSocket == EquippedWeapon->GetWeaponData().HolsterSocket) {
	//				ABaseWeapon* OldHolsteredWeapon = Slot.Weapon;
	//				Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
	//				OldHolsteredWeapon->Destroy();
	//				EquipWeapon(Slot.Weapon);
	//				// TODO: spawn this old holstered weapon on the floor with old data
	//				break;
	//			}
	//		}
	//	}
	//}
	OverlappedWeapon = nullptr;
	WeaponToPickup->Destroy();
}

bool ABaseCharacter::Server_PickUp_Validate(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner) {
	return true;
}

ABaseWeapon* ABaseCharacter::SpawnPickedUpWeapon(FWeaponData Data, FName HolsterSocket, AActor* WeaponOwner) {
	USceneComponent* CharacterMesh = Cast<USceneComponent>(FindComponentByClass<USkeletalMeshComponent>());
	if (CharacterMesh == nullptr) return nullptr;

	ABaseWeapon* Weapon = GetWorld()->SpawnActor<ABaseWeapon>(Data.WeaponBlueprint, CharacterMesh->GetComponentTransform());	
	if (Weapon == nullptr) return nullptr;
	Weapon->SetOwner(WeaponOwner);
	Data.HolsterSocket = HolsterSocket;
	Weapon->SetWeaponData(Data);
	Weapon->DisablePickUp();
	Weapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Data.HolsterSocket);

	return Weapon;
}

bool ABaseCharacter::FillEmptyWeaponSlot(struct FWeaponData Data) {
	bool FilledSlot = false;
	for (FWeaponSlot& Slot : WeaponSlots) {
		if (Slot.Weapon == nullptr) {
			Slot.Weapon = SpawnPickedUpWeapon(Data, Slot.HolsterSocket, this);
			FilledSlot = true;
			break;
		}
	}
	return FilledSlot;
}



/// Equip Weapon logic
//TODO: Use enums or something for different weapon types?

void ABaseCharacter::ActionBar1() {
	EquipWeapon(WeaponSlots[0].Weapon);
}

void ABaseCharacter::ActionBar2() {
	EquipWeapon(WeaponSlots[1].Weapon);
}

void ABaseCharacter::EquipWeapon(ABaseWeapon* Weapon) {
	// If no weapon, don't perform any animation etc either
	if (Weapon == nullptr) return;

	if (EquippedWeapon == nullptr) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
	else if (EquippedWeapon == Weapon || Weapon == nullptr) {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}

	if (Role < ROLE_Authority) {
		Server_EquipWeapon(Weapon);
	}
	if (Role == ROLE_Authority) {
		Multicast_WeaponEquip();
	}
	WeaponToEquip = Weapon;
	bSwappingWeapon = true;
}

void ABaseCharacter::Server_EquipWeapon_Implementation(ABaseWeapon* Weapon) {
	EquipWeapon(Weapon);
}

bool ABaseCharacter::Server_EquipWeapon_Validate(ABaseWeapon* Weapon) { return true; }

void ABaseCharacter::Multicast_WeaponEquip_Implementation() {
	WeaponEquipEvent();
}

void ABaseCharacter::WeaponEquipEvent_Implementation() {}

void ABaseCharacter::HandleEquip() {
	if (!HasAuthority()) return;
	USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
	if (CharacterMesh == nullptr) return;

	// Safety check. Stop function if both are nullptr
	if (EquippedWeapon == nullptr && WeaponToEquip == nullptr) return;
	// when no weapon equiped, equip weapon to equip
	if (EquippedWeapon == nullptr && WeaponToEquip != nullptr) {
		WeaponToEquip->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		EquippedWeapon = WeaponToEquip;
		return;
	}

	// When trying to swap to  the same weapon, holster weapon instead
	if (WeaponToEquip == EquippedWeapon) {
		EquippedWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedWeapon->GetWeaponData().HolsterSocket);
		EquippedWeapon = nullptr;
		return;
	}

	// When trying to swap to another weapon while having one equipped
	if (EquippedWeapon && WeaponToEquip != nullptr) {
		EquippedWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedWeapon->GetWeaponData().HolsterSocket);
		WeaponToEquip->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		EquippedWeapon = WeaponToEquip;
	}
}




/// Possession
void ABaseCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("possess character"));
}

void ABaseCharacter::UnPossessed() {
	Super::UnPossessed();
	SetAutonomousProxy(false);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("UNpossess character"));
}



/// Fire
void ABaseCharacter::Fire() {
	if (EquippedWeapon == nullptr) return;
	if (!bSwappingWeapon) {
		EquippedWeapon->Fire();
	}
}