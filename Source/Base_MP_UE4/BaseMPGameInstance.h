// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MainMenuInterface.h"
#include "BaseMPGameInstance.generated.h"

class UUserWidget;
class APlayerController;
/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UBaseMPGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()

// Variables
private:
	TSubclassOf<UUserWidget> MainMenuClass;

// Functions
public:
	UBaseMPGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);

};
