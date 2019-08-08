// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystemGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MenuSystem/GameMenu/GameMenu.h"
#include "MenuSystem/In-GameMenu/InGameMenu.h"
#include "MenuSystem/GameMenu/ServersMenu/ServersMenu.h"
#include "MenuSystem/MenuWidget.h"

//const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
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
	World->ServerTravel(LobbyMap + "?listen");
}

void UMenuSystemGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UMenuSystemGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			SessionSettings.bIsLANMatch = true;
		}
		else {
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(
			SERVER_NAME_SETTINGS_KEY, 
			DesiredServerName, 
			EOnlineDataAdvertisementType::ViaOnlineServiceAndPing
		);

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
}

void UMenuSystemGameInstance::OnFindSessionsComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"));
	if (Success && SessionSearch.IsValid() && ServersMenu != nullptr) {
		TArray<FServerData> ServerDataList;
		for (const auto& result : SessionSearch->SearchResults) {
			FServerData Data;
			Data.ServerName = result.GetSessionIdStr();
			Data.MaxPlayerCount = result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayerCount = Data.MaxPlayerCount - result.Session.NumOpenPublicConnections;
			Data.HostUserName = result.Session.OwningUserName;

			FString ServerName;
			if (result.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName)) {
				Data.ServerName = ServerName;
			}
			else {
				Data.ServerName = "Could not find name.";
			}
			ServerDataList.Add(Data);
		}
		ServersMenu->SetServerList(ServerDataList);
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

void UMenuSystemGameInstance::Host(FString ServerName) {
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else {
			CreateSession();
		}
	}
}

void UMenuSystemGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}

void UMenuSystemGameInstance::StartSession() {
	if (SessionInterface.IsValid()) {
		SessionInterface->StartSession(NAME_GameSession);
	}
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

void UMenuSystemGameInstance::QuitGame() {
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}