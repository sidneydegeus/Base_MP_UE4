// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Character/BaseCharacter.h"
#include "LockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool bIsBeingLockedOn;
	bool bIsLockedOnTarget;

protected:
	UPROPERTY(EditDefaultsOnly)
	float MinLockOnDistance = 1000.f;

	float ClosestTargetDistance;

	UPROPERTY(EditDefaultsOnly)
	float BreakLineOfSightDelay = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float DistanceLockOffDelay = 2.f;

	UPROPERTY()
	class ABaseCharacter* ParentCharacter;

	UPROPERTY()
	class ABaseCharacter* CurrentTarget;

	bool bDoOnce;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* fCurve;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> LockOnCollisionTypes;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> ActorsToIgnore;

	// Possibly rename? limit lockon to a basecharacter for now
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseCharacter> CharacterTypeLockOnClass = ABaseCharacter::StaticClass();

	class UTimelineComponent* Timeline;

private:
	FTimerHandle BreakLineOfSightHandle;
	FTimerHandle DistanceLockOffHandle;




public:	
	ULockOnComponent();
	void LockOn();

	UFUNCTION()
	void LockOff();
	void DisplayLockOnVisual();

protected:
	virtual void BeginPlay() override;

	FOnTimelineFloat InterpFunction{};
	UFUNCTION()
	void TimelineCallback(float val);

private:
	void LockOnNearestTarget();

	void BreakLineOfSight();
	void DistanceLockOff();
	void DistanceLockOffDelayFunc();

	void LockOnCameraRotation();
};
