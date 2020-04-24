// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"

#include "Net/UnrealNetwork.h"
#include "BaseMP_PlayerController.h"
#include "GenericComponents/SpawnPoint.h"
#include "Weapon/BaseWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/TankUI.h"

ATurretBase::ATurretBase() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT(
	//	"/Game/Ingame-UI/Vehicle/Tank_WBP"
	//));
	//if (!ensure(GameMenuBPClass.Class != nullptr)) return;
	//VehicleUI = GameMenuBPClass.Class;
}

void ATurretBase::BeginPlay() {
	Super::BeginPlay();
	//CurrentHealth = MaxHealth;
}

void ATurretBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATurretBase::Fire);
}

void ATurretBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATurretBase::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	//ABaseMP_PlayerController* ABase_PlayerController = Cast<ABaseMP_PlayerController>(NewController);
	//if (ABase_PlayerController == nullptr) return;


	////TODO: set weapon instead

	//auto Weapon = GetWeapon();
	//if (Weapon == nullptr) return;
	//ABase_PlayerController->SetWeapon(Weapon);
	//Client_SetWeapon(ABase_PlayerController);
	// CLIENT SET WEAPON
}

void ATurretBase::UnPossessed() {
	//UTankAimingComponent* TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	//if (TankAimingComponent == nullptr) return;
	//TankAimingComponent->Deactivate();
	//ABaseMP_PlayerController* ABase_PlayerController = Cast<ABaseMP_PlayerController>(Controller);
	//if (ABase_PlayerController == nullptr) return;
	//ABase_PlayerController->SetWeapon(nullptr);
	//TODO: maybe change the aiming component setting to the controller itself by overwriting UnPosses()
	//ABase_PlayerController->SetAimingComponent(nullptr);
	//above needs to be executed first, so that the controller can be cleaned up.
	Super::UnPossessed();
}

//void ATurretBase::Client_SetWeapon_Implementation(ABaseMP_PlayerController* ABase_PlayerController) {
//	auto Weapon = GetWeapon();
//	if (Weapon == nullptr) return;
//	ABase_PlayerController->SetWeapon(Weapon);
//}

float ATurretBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		//OnDeath.Broadcast();
	}

	return DamageToApply;
}

float ATurretBase::GetHealthPercentage() const {
	return (float)CurrentHealth / (float)MaxHealth;
}

ABaseWeapon* ATurretBase::GetWeapon() const {
	auto test = GetComponents();
	for (UActorComponent* Child : test) {
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto Weapon = Cast<ABaseWeapon>(SpawnedChild);
		if (Weapon) return Weapon;
	}
	return nullptr;
}


void ATurretBase::SetupUI() {
	if (UI) {
		auto TankUI = Cast<UTankUI>(UI);
		if (TankUI) {
			TankUI->Weapon = Weapon;
		}
	}
	Super::SetupUI();
}

void ATurretBase::Fire() {
	//auto Weapon = GetWeapon();
	if (Weapon == nullptr) return;
	Weapon->Fire();
}