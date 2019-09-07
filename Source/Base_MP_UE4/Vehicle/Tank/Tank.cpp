// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Net/UnrealNetwork.h"
#include "BaseMP_PlayerController.h"
#include "TankAimingComponent.h"
#include "UObject/ConstructorHelpers.h"

//void ATank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ATank, ReplicatedTransform);
//}

ATank::ATank() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT(
	//	"/Game/Ingame-UI/Vehicle/Tank_WBP"
	//));
	//if (!ensure(GameMenuBPClass.Class != nullptr)) return;
	//VehicleUI = GameMenuBPClass.Class;
}

void ATank::BeginPlay() {
	Super::BeginPlay();
	//CurrentHealth = MaxHealth;
}

void ATank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//if (HasAuthority()) {
	//	ReplicatedTransform = GetActorTransform();
	//}
}

void ATank::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	UTankAimingComponent* TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (TankAimingComponent == nullptr) return;
	TankAimingComponent->Activate(true);

	ABaseMP_PlayerController* ABase_PlayerController = Cast<ABaseMP_PlayerController>(NewController);
	if (ABase_PlayerController == nullptr) return;
	ABase_PlayerController->SetAimingComponent(TankAimingComponent);
}

void ATank::UnPossessed() {
	UTankAimingComponent* TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (TankAimingComponent == nullptr) return;
	TankAimingComponent->Deactivate();
	ABaseMP_PlayerController* ABase_PlayerController = Cast<ABaseMP_PlayerController>(Controller);
	if (ABase_PlayerController == nullptr) return;
	ABase_PlayerController->SetAimingComponent(nullptr);
	//above needs to be executed first, so that the controller can be cleaned up.
	//TODO: maybe change the aiming component setting to the controller itself by overwriting UnPosses()
	Super::UnPossessed();
}

//void ATank::OnRep_ReplicatedTransform() {
//	SetActorTransform(ReplicatedTransform);
//}

float ATank::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		OnDeath.Broadcast();
	}

	return DamageToApply;
}

float ATank::GetHealthPercentage() const {
	return (float)CurrentHealth / (float)MaxHealth;
}