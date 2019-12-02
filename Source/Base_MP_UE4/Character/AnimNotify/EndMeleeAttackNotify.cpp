// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMeleeAttackNotify.h"
#include "Engine.h"
#include "Character/BaseCharacter.h"

// set attacking state to none
void UEndMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp == nullptr && MeshComp->GetOwner() == nullptr) return;
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	Character->OnStopAttack();
}