// Fill out your copyright notice in the Description page of Project Settings.


#include "ManeuverEndNotify.h"
#include "Engine.h"
#include "Character/BaseCharacter.h"

void UManeuverEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp == nullptr && MeshComp->GetOwner() == nullptr) return;
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	Character->OnManeuverEnd();
}
