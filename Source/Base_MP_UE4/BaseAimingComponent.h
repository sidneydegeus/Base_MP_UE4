// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAimingComponent.generated.h"


UCLASS()
class BASE_MP_UE4_API UBaseAimingComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Firing")
		int32 Ammo = 3;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//	TSubclassOf<AProjectile> ProjectileBlueprint;

// Functions
public:	
	UBaseAimingComponent();
	virtual void AimAt(FVector HitLocation);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	

		
};
