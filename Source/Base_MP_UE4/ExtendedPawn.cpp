// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedPawn.h"

#include "Blueprint/UserWidget.h"
#include "BaseMP_PlayerController.h"

void AExtendedPawn::SetupUIEvent_Implementation() {}
void AExtendedPawn::UpdateUIEvent_Implementation() {}

void AExtendedPawn::Client_PossessedBy_Implementation(APlayerController* PlayerController) {
	if (PlayerController == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("1"));
	if (UIClass == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("2"));
	UI = CreateWidget<UUserWidget>(PlayerController, UIClass);
	UI->AddToViewport();
	SetupUIEvent();
}

void AExtendedPawn::Client_UnPossessed_Implementation() {
	ABaseMP_PlayerController* PlayerController = Cast<ABaseMP_PlayerController>(GetController());
	if (PlayerController == nullptr) return;
	if (UI == nullptr) return;
	UI->RemoveFromViewport();
}