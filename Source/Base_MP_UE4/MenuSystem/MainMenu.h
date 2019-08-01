// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
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
		class UWidgetSwitcher* MenuSwitcher;

	//Main menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenu_HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenu_JoinButton;


	//Join Menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_BackButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressTextBox;



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

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void OpenJoinMenu();

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	bool bAddDynamicsMainMenu();
	bool bAddDynamicsJoinMenu();
};
