// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/DamageType.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ABaseProjectile::ABaseProjectile() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(CollisionMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABaseProjectile::BeginPlay() {
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
}

void ABaseProjectile::LaunchProjectile(float Speed) {
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void ABaseProjectile::ResolveHit_Implementation() {
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABaseProjectile::OnTimerExpire, DestroyDelay, false);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor == GetOwner()) return;
	ResolveHit();

	if (HasAuthority()) {
		UGameplayStatics::ApplyRadialDamage(
			this,
			ProjectileDamage,
			GetActorLocation(),
			ExplosionForce->Radius,
			UDamageType::StaticClass(),
			TArray<AActor*>()
		);
	}
}

void ABaseProjectile::OnTimerExpire() {
	Destroy();
}