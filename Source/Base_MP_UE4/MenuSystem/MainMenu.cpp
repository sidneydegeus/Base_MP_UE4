// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/ServersMenu/ServersMenu.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> JoinMenuBPClass(TEXT("/Game/MenuSystem/WBP_JoinMenu"));
	if (!ensure(JoinMenuBPClass.Class != nullptr)) return;
	JoinMenuClass = JoinMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ServersMenuBPClass(TEXT("/Game/MenuSystem/WBP_ServersMenu"));
	if (!ensure(ServersMenuBPClass.Class != nullptr)) return;
	ServersMenuClass = ServersMenuBPClass.Class;
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(ServersButton != nullptr)) return false;
	ServersButton->OnClicked.AddDynamic(this, &UMainMenu::OpenServersMenu);

	if (!ensure(ExitGameButton != nullptr)) return false;
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::HostServer() {
	if (Menu->GetMainMenuInterface() != nullptr) {
		Menu->GetMainMenuInterface()->Host();
	}
}

void UMainMenu::OpenJoinMenu() {
	Menu->OpenSubMenuWidget(JoinMenuClass);
}

void UMainMenu::OpenServersMenu() {
	Menu->OpenSubMenuWidget(ServersMenuClass);
}

void UMainMenu::ExitGame() {
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

