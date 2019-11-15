// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRangedWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/BaseProjectile.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Character/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void ABaseRangedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseRangedWeapon, Ammo);
}

ABaseRangedWeapon::ABaseRangedWeapon() {
	WeaponType = EWeaponType::Ranged;
}

void ABaseRangedWeapon::AimAt(FHitResult HitResult, bool bResultHit) {
	FVector MeshSocketLocation = SpawnProjectileLocation();

	FVector Location = UKismetMathLibrary::SelectVector(HitResult.ImpactPoint, HitResult.TraceEnd, bResultHit);
	FRotator TransformRotation = UKismetMathLibrary::FindLookAtRotation(MeshSocketLocation, Location);
	FVector TransformScale = FVector(1, 1, 1);

	ProjectileSpawnTransform = FTransform(TransformRotation, MeshSocketLocation, TransformScale);
}

/// Projectile
bool ABaseRangedWeapon::SpawnProjectile(FTransform Transform) {
	//auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileBlueprint, Transform);
	//if (Projectile == nullptr) return false;
	//if (GetOwner() != nullptr) Projectile->SetOwner(GetOwner());
	//UGameplayStatics::FinishSpawningActor(Projectile, Transform);

	FActorSpawnParameters F;
	F.Owner = GetOwner();
	auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(
		ProjectileBlueprint,
		Transform,
		F
		);
	if (Projectile == nullptr) return false;
	Projectile->LaunchProjectile(FVector::ForwardVector, LaunchSpeed);
	return true;
}


//TODO: maybe remove OR rename
FVector ABaseRangedWeapon::SpawnProjectileLocation() {
	////INTENDED: Override in child for functionality
	FindMesh();
	if (Mesh == nullptr) return FVector();
	return Mesh->GetSocketLocation(FName("Muzzle"));
}



/// Fire
void ABaseRangedWeapon::Fire() {
	//AimAt(GetHitResult());
	Server_Fire(ProjectileSpawnTransform);
}

void ABaseRangedWeapon::Server_Fire_Implementation(FTransform Transform) {
	if (ProjectileBlueprint == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No projectile Blueprint!!!!"));
		return;
	}
	if (!SpawnProjectile(Transform)) return;;

	Server_SetAmmo(Ammo - 1);
	//TODO: not sure but this should probably be changed for other weapons
	LastFireTime = FPlatformTime::Seconds();
}



/// Ammo 
void ABaseRangedWeapon::Server_SetAmmo_Implementation(uint32 Amount) {
	Ammo = Amount;
}
