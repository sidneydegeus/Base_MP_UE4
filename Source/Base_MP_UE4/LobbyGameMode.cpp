// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "Engine/Engine.h"
#include "TimerManager.h"

#include "MenuSystem/MenuSystemGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 2) {
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame() {
	auto GameInstance = Cast<UMenuSystemGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel(FString::Printf(TEXT("%s?listen"), *GameInstance->GameMap));
}