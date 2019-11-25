// Fill out your copyright notice in the Description page of Project Settings.


#include "BeginMeleeAttackNotify.h"
#include "Engine.h"
#include "Character/BaseCharacter.h"

// set  attacking staet teo start

void UBeginMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UE_LOG(LogTemp, Warning, TEXT("begin notify sword"));
	if (MeshComp == nullptr && MeshComp->GetOwner() == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("begin notify sword 1st if"));
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("begin notify sword 2nd if"));
	Character->SetIsAttacking(true);
}