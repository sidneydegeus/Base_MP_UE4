// Fill out your copyright notice in the Description page of Project Settings.


#include "TankShell.h"
#include "Classes/GameFramework/DamageType.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"

void ATankShell::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (!HasAuthority() || OtherActor == GetOwner()) return;
	if (!ExplosionForce) return;
	UGameplayStatics::ApplyRadialDamage(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>()
	);
	ResolveHit();
}