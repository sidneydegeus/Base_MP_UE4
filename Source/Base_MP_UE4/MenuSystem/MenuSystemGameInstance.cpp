// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystemGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/GameMenu/GameMenu.h"
#include "MenuSystem/In-GameMenu/InGameMenu.h"
#include "MenuSystem/GameMenu/ServersMenu/ServersMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UMenuSystemGameInstance::UMenuSystemGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT(
		"/Game/MenuSystem/Menus/GameMenu/WBP_GameMenu"
	));
	if (!ensure(GameMenuBPClass.Class != nullptr)) return;
	GameMenuClass = GameMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT(
		"/Game/MenuSystem/Menus/In-GameMenu/WBP_InGameMenu"
	));
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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnJoinSessionComplete);

		}
	}
}


void UMenuSystemGameInstance::LoadGameMenuWidget() {
	if (!ensure(GameMenuClass != nullptr)) return;
	UGameMenu* GameMenu = CreateWidget<UGameMenu>(this, GameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;

	GameMenu->Setup();
	GameMenu->SetMainMenuInterface(this);
}

void UMenuSystemGameInstance::LoadInGameMenuWidget() {
	if (!ensure(InGameMenuClass != nullptr)) return;
	UInGameMenu* InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMainMenuInterface(this);
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
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
		//SessionSettings.bIsLANMatch = true;
		//SessionSettings.NumPublicConnections = 4;
		//SessionSettings.bShouldAdvertise = true;
		//SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UMenuSystemGameInstance::OnFindSessionsComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"));
	if (Success && SessionSearch.IsValid() && ServersMenu != nullptr) {
		TArray<FString> ServerNames;
		for (const auto& result : SessionSearch->SearchResults) {
			ServerNames.Add(result.GetSessionIdStr());
		}
		ServersMenu->SetServerList(ServerNames);
	}
}

void UMenuSystemGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));


	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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

void UMenuSystemGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMenuSystemGameInstance::RefreshServerList(class UServersMenu* ToSetServersMenu) {
	ServersMenu = ToSetServersMenu;
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}