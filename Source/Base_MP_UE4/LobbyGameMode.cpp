// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 2) {
		//GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
		UE_LOG(LogTemp, Warning, TEXT("Number is reached 2"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame() {
	//auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	//if (GameInstance == nullptr) return;

	//GameInstance->StartSession();

	//UWorld* World = GetWorld();
	//if (!ensure(World != nullptr)) return;

	//bUseSeamlessTravel = true;
	//World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}