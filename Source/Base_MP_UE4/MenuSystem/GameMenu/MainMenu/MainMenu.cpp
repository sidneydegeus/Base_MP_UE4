// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/GameMenu/ServersMenu/ServersMenu.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> HostMenuBPClass(TEXT(
		"/Game/MenuSystem/Menus/GameMenu/HostMenu/WBP_HostMenu"
	));
	if (!ensure(HostMenuBPClass.Class != nullptr)) return;
	HostMenuClass = HostMenuBPClass.Class;

	//ConstructorHelpers::FClassFinder<UUserWidget> JoinMenuBPClass(TEXT(
	//	"/Game/MenuSystem/Menus/GameMenu/JoinMenu/WBP_JoinMenu"
	//));
	//if (!ensure(JoinMenuBPClass.Class != nullptr)) return;
	//JoinMenuClass = JoinMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ServersMenuBPClass(TEXT(
		"/Game/MenuSystem/Menus/GameMenu/ServersMenu/WBP_ServersMenu"		
	));
	if (!ensure(ServersMenuBPClass.Class != nullptr)) return;
	ServersMenuClass = ServersMenuBPClass.Class;
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	//if (!ensure(JoinButton != nullptr)) return false;
	//JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(ServersButton != nullptr)) return false;
	ServersButton->OnClicked.AddDynamic(this, &UMainMenu::OpenServersMenu);

	if (!ensure(ExitGameButton != nullptr)) return false;
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::HostServer() {
	if (Menu->GetMenuInterface() != nullptr) {
		Menu->OpenSubMenuWidget(HostMenuClass);
	}
}

//void UMainMenu::OpenJoinMenu() {
//	if (Menu->GetMenuInterface() != nullptr) {
//		Menu->OpenSubMenuWidget(JoinMenuClass);
//	}
//}

void UMainMenu::OpenServersMenu() {
	if (Menu->GetMenuInterface() != nullptr) {
		Menu->OpenSubMenuWidget(ServersMenuClass);
	}
}

void UMainMenu::QuitGame() {
	if (Menu->GetMenuInterface() != nullptr) {
		Menu->GetMenuInterface()->QuitGame();
	}
}

