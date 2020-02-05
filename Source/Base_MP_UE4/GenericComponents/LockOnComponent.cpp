// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Engine.h"

ULockOnComponent::ULockOnComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	InterpFunction.BindUFunction(this, FName("TimelineCallback"));
}


void ULockOnComponent::BeginPlay(){
	Super::BeginPlay();
	ParentCharacter = Cast<ABaseCharacter>(GetOwner());

	if (fCurve) {
		Timeline->SetLooping(true);
		Timeline->AddInterpFloat(fCurve, InterpFunction);
	}
}


void ULockOnComponent::LockOn() {
	ClosestTargetDistance = MinLockOnDistance;
	if (bIsLockedOnTarget) {
		LockOff();
	}
	else {
		LockOnNearestTarget();
	}
}


void ULockOnComponent::LockOff() {
	bIsLockedOnTarget = false;
	Timeline->Stop();
	if (CurrentTarget) {
		CurrentTarget->GetLockOnComponent()->DisplayLockOnVisual();
		CurrentTarget = NULL;
	}
}

void ULockOnComponent::LockOnNearestTarget() {
	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsOfClass(this, CharacterTypeLockOnClass, ReturnedActors);

	ABaseCharacter* EnemyElement;
	for (AActor* Actor : ReturnedActors) {
		EnemyElement = Cast<ABaseCharacter>(Actor);
		if (EnemyElement && EnemyElement->bIsAlive) {
			FHitResult HitResult;
			UKismetSystemLibrary::LineTraceSingleForObjects(
				this,
				GetOwner()->GetActorLocation(),
				EnemyElement->GetActorLocation(),
				LockOnCollisionTypes,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true
			);
			if (EnemyElement == Cast<ABaseCharacter>(HitResult.Actor)) {
				float DistanceFromPlayer = EnemyElement->DistanceToCharacter(ParentCharacter);
				if (DistanceFromPlayer < ClosestTargetDistance) {
					ClosestTargetDistance = DistanceFromPlayer;
					CurrentTarget = EnemyElement;
					bDoOnce = false;
				}
			}
		}
	}

	if (ClosestTargetDistance < MinLockOnDistance && CurrentTarget) {
		bIsLockedOnTarget = true;
		CurrentTarget->GetLockOnComponent()->DisplayLockOnVisual();
		Timeline->Play();
	}
}

void ULockOnComponent::DisplayLockOnVisual() {
	bIsBeingLockedOn = (bIsBeingLockedOn) ? false : true;
	ParentCharacter->GetMesh()->SetRenderCustomDepth(bIsBeingLockedOn);
}

void ULockOnComponent::BreakLineOfSight() {
	if (bIsLockedOnTarget) {
		TArray<AActor*> ReturnedActors;
		UGameplayStatics::GetAllActorsOfClass(this, CharacterTypeLockOnClass, ReturnedActors);
		ReturnedActors.Remove(CurrentTarget);

		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingleForObjects(
			this,
			ParentCharacter->GetCapsuleComponent()->K2_GetComponentLocation(),
			CurrentTarget->GetCapsuleComponent()->K2_GetComponentLocation(),
			LockOnCollisionTypes,
			false,
			ReturnedActors,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		if (CurrentTarget == Cast<ABaseCharacter>(HitResult.Actor)) {
			GetWorld()->GetTimerManager().SetTimer(BreakLineOfSightHandle, this, &ULockOnComponent::LockOff, BreakLineOfSightDelay, false);
		}
	}
}

void ULockOnComponent::DistanceLockOff() {
	if (bIsLockedOnTarget && CurrentTarget) {
		float DistanceTo = CurrentTarget->DistanceToCharacter(ParentCharacter);
		if (DistanceTo < MinLockOnDistance) {
			GetWorld()->GetTimerManager().SetTimer(DistanceLockOffHandle, this, &ULockOnComponent::DistanceLockOffDelayFunc, DistanceLockOffDelay, false);
		}
	}
}

void ULockOnComponent::DistanceLockOffDelayFunc() {
	if (!bDoOnce) {
		LockOff();
		bDoOnce = true;
	}
}

void ULockOnComponent::TimelineCallback(float val) {
	BreakLineOfSight();
	DistanceLockOff();
	LockOnCameraRotation();
}

void ULockOnComponent::LockOnCameraRotation() {
	AController* Controller = ParentCharacter->GetController();

	FRotator CurrentControlRotation = Controller->GetControlRotation();
	FRotator NewRotation;
	NewRotation.Roll = CurrentControlRotation.Roll;
	NewRotation.Pitch = CurrentControlRotation.Pitch;
	NewRotation.Yaw = UKismetMathLibrary::RInterpTo(
		CurrentControlRotation, 
		UKismetMathLibrary::FindLookAtRotation(ParentCharacter->GetFollowCamera()->K2_GetComponentLocation(), CurrentTarget->GetActorLocation()),
		UGameplayStatics::GetWorldDeltaSeconds(this), 
		7.f
	).Yaw;

	Controller->SetControlRotation(NewRotation);
}