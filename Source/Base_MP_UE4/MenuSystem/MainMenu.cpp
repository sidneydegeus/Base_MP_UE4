// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!bAddDynamicsMainMenu()) return false;
	if (!bAddDynamicsJoinMenu()) return false;
	if (!bAddDynamicsServerMenu()) return false;

	return true;
}

void UMainMenu::HostServer() {
	if (MainMenuInterface != nullptr) {
		MainMenuInterface->Host();
	}
}

void UMainMenu::JoinServer() {
	if (MainMenuInterface != nullptr) {
		if (!ensure(ServerRowClass != nullptr)) return;
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;
		ServerList->AddChild(ServerRow);
	}
}

void UMainMenu::JoinIP() {
	if (MainMenuInterface != nullptr) {
		if (!ensure(IPAddressTextBox != nullptr)) return;
		const FString& Address = IPAddressTextBox->GetText().ToString();
		MainMenuInterface->Join(Address);
	}
}

void UMainMenu::ExitGame() {
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenServersMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(ServersMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(ServersMenu);
}

bool UMainMenu::bAddDynamicsMainMenu() {
	if (!ensure(MainMenu_HostButton != nullptr)) return false;
	MainMenu_HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(MainMenu_JoinButton != nullptr)) return false;
	MainMenu_JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(MainMenu_ServersButton != nullptr)) return false;
	MainMenu_ServersButton->OnClicked.AddDynamic(this, &UMainMenu::OpenServersMenu);

	if (!ensure(MainMenu_ExitGameButton != nullptr)) return false;
	MainMenu_ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

bool UMainMenu::bAddDynamicsJoinMenu() {
	if (!ensure(JoinMenu_JoinButton != nullptr)) return false;
	JoinMenu_JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinIP);

	if (!ensure(JoinMenu_BackButton != nullptr)) return false;
	JoinMenu_BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

bool UMainMenu::bAddDynamicsServerMenu() {
	if (!ensure(ServersMenu_JoinButton != nullptr)) return false;
	ServersMenu_JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(ServersMenu_BackButton != nullptr)) return false;
	ServersMenu_BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}