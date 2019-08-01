// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

// Variables
protected:
	IMainMenuInterface* MainMenuInterface;

// functions
public:
	void SetMainMenuInterface(IMainMenuInterface* MainMenuInterface);
	void Setup();

protected:
	void TearDownWidget();

private:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
