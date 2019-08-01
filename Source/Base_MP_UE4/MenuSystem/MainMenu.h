// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

//Variables
private:
	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	IMainMenuInterface* MainMenuInterface;

//Functions
public:
	void SetMainMenuInterface(IMainMenuInterface* MainMenuInterface);
	void Setup();

protected:
	virtual bool Initialize() override;


private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
