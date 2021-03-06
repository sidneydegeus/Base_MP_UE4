// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class BASE_MP_UE4_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
//Variables
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* ProjectileCollision = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class URadialForceComponent* ExplosionForce = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileDamage = 20.f;


//Functions
public:	
	ABaseProjectile();
	void LaunchProjectile(FVector ForwardVector, float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		void ResolveHit();
		void ResolveHit_Implementation();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

private:
	void OnTimerExpire();

};
