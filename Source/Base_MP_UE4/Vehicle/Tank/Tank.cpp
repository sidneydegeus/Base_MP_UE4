// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Net/UnrealNetwork.h"

//void ATank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ATank, ReplicatedTransform);
//}

ATank::ATank() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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