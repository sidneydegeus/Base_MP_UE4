// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_MP_UE4GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ALobbyGameMode : public ABase_MP_UE4GameMode
{
	GENERATED_BODY()
	
//Variables
private:
	uint16 NumberOfPlayers = 0;
	FTimerHandle GameStartTimer;

//Functions

public:
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

private:
	void StartGame();

};
