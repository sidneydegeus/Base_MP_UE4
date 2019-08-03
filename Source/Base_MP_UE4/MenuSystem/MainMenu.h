// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMainMenu : public UMenuWidget
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

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenu_ExitGameButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenu_ServersButton;
	


	//Join Menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenu_BackButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressTextBox;

	//Server Menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* ServersMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UButton* ServersMenu_JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ServersMenu_BackButton;

	TSubclassOf<class UUserWidget> ServerRowClass;

//Functions
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;


private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void JoinIP();

	UFUNCTION()
		void ExitGame();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenServersMenu();

	bool bAddDynamicsMainMenu();
	bool bAddDynamicsJoinMenu();
	bool bAddDynamicsServerMenu();
};
