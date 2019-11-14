// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/DamageType.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SphereComponent.h"

// Sets default values
ABaseProjectile::ABaseProjectile() {
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);
	//Root->SetNotifyRigidBodyCollision(true);
	//Root->SetVisibility(false);
	//Root->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(FName("Projectile Collision"));
	ProjectileCollision->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABaseProjectile::BeginPlay() {
	Super::BeginPlay();
	//CollisionMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
}

void ABaseProjectile::LaunchProjectile(FVector ForwardVector, float Speed) {
	ProjectileMovement->SetVelocityInLocalSpace(ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void ABaseProjectile::ResolveHit_Implementation() {
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	Root->DestroyComponent();

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