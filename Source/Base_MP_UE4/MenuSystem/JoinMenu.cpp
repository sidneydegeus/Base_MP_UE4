// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinMenu.h"

#include "Components/Button.h"

UJoinMenu::UJoinMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	//
}

bool UJoinMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UJoinMenu::JoinIP);
	
	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UJoinMenu::Back);

	return true;
}

void UJoinMenu::JoinIP() {
	UE_LOG(LogTemp, Warning, TEXT("Join IP"));
}