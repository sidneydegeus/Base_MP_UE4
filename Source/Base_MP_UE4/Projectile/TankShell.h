// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "TankShell.generated.h"

/**
 * 
 */
UCLASS(abstract)
class BASE_MP_UE4_API ATankShell : public ABaseProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult) override;
};
