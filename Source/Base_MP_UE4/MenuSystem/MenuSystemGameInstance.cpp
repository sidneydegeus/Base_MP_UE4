// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystemGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UMenuSystemGameInstance::UMenuSystemGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MainMenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UMenuSystemGameInstance::Init() {
	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *InGameMenuClass->GetName());
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnFindSessionsComplete);
		
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid()) {
				SessionSearch->bIsLanQuery = true;
				UE_LOG(LogTemp, Warning, TEXT("Start finding sessions"));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
}


void UMenuSystemGameInstance::LoadMainMenuWidget() {
	if (!ensure(MainMenuClass != nullptr)) return;
	UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();
	MainMenu->SetMainMenuInterface(this);
}

void UMenuSystemGameInstance::LoadInGameMenuWidget() {
	if (!ensure(InGameMenuClass != nullptr)) return;
	UInGameMenu* InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMainMenuInterface(this);
}

void UMenuSystemGameInstance::Host() {
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession();
		}
	}
}

void UMenuSystemGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel(HostMap + "?listen");
}

void UMenuSystemGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UMenuSystemGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UMenuSystemGameInstance::OnFindSessionsComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"));
	if (Success && SessionSearch.IsValid()) {
		// post shit
		auto results = SessionSearch->SearchResults;
		for (const auto& result : results) {
			UE_LOG(LogTemp, Warning, TEXT("Found Session Names: %s"), *result.GetSessionIdStr());		
		}

	}
}

void UMenuSystemGameInstance::Join(const FString& Address) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}