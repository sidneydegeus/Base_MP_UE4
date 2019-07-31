// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMPGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UBaseMPGameInstance::UBaseMPGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MainMenuClass = MenuBPClass.Class;
}

void UBaseMPGameInstance::Init()  {
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());
}


void UBaseMPGameInstance::LoadMainMenu() {
	if (!ensure(MainMenuClass != nullptr)) return;

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(MainMenu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	MainMenu->bIsFocusable = true;
}

void UBaseMPGameInstance::Host() {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/MultiplayerSessionMap?listen");
}

void UBaseMPGameInstance::Join(const FString& Address) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}