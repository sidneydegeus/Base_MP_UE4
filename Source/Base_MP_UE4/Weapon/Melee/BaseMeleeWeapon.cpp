// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeWeapon.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Classes/Kismet/GameplayStatics.h"

ABaseMeleeWeapon::ABaseMeleeWeapon() {
	WeaponType = EWeaponType::Melee;
	HitBoxComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("HitBox"));
	HitBoxComponent->AttachToComponent(RootPlaceholderComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackRange = 100.f;
}

void ABaseMeleeWeapon::BeginPlay() {
	Super::BeginPlay();
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMeleeWeapon::BeginOverlap);
}



void ABaseMeleeWeapon::Server_Fire_Implementation(FTransform Transform) {
	// call owner and perform multicast  animation shit
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (BaseCharacter) {
		BaseCharacter->RequestWeaponAnimation();
	}
}

void ABaseMeleeWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (!HasAuthority() || OtherActor == GetOwner()) return;
	auto Actor = ActorsHit.Find(OtherActor);
	if (Actor == INDEX_NONE) {
		TSubclassOf<UDamageType> DamageType;
		UGameplayStatics::ApplyDamage(
			OtherActor,
			FMath::FRandRange(MinimumDamage, MaximumDamage),
			GetInstigatorController(),
			this,
			DamageType
		);
		ActorsHit.Add(OtherActor);
	}
}

void ABaseMeleeWeapon::ActivateHitBoxCollision() {
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseMeleeWeapon::DeactivateHitBoxCollision() {
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void ABaseMeleeWeapon::ResetActorsHit() {
	ActorsHit.Empty();
}

// TODO:
// reset actorshit