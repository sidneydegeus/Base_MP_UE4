// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "Lobby.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ULobby : public UMenuWidget
{
	GENERATED_BODY()
	
//Variables
private:
	//TSubclassOf<class USubMenuWidget> MainMenuClass;

	//Functions
public:
	ULobby(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;
};
