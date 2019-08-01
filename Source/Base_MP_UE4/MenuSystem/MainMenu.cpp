// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::SetMainMenuInterface(IMainMenuInterface* MainMenuInterface) {
	this->MainMenuInterface = MainMenuInterface;
}

void UMainMenu::Setup() {
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::HostServer() {
	if (MainMenuInterface != nullptr) {
		MainMenuInterface->Host();
	}
}

void UMainMenu::JoinServer() {
	UE_LOG(LogTemp, Warning, TEXT("joining server!!"));
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
	RemoveFromViewport();
	auto* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	auto* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly inputMode;
	PlayerController->SetInputMode(inputMode);
	PlayerController->bShowMouseCursor = false;

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}