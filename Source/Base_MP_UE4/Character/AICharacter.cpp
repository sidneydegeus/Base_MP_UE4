// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericComponents/ExitPawnComponent.h"

AAICharacter::AAICharacter() {
	PrimaryActorTick.bCanEverTick = true;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AAICharacter::OnDeath() {
	Super::OnDeath();
	if (ExitComponent) ExitComponent->UnpossesAI();
}