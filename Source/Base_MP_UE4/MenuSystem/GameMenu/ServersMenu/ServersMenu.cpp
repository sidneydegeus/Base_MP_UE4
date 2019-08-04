// Fill out your copyright notice in the Description page of Project Settings.


#include "ServersMenu.h"

#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/GameMenu/ServersMenu/ServerRow.h"

UServersMenu::UServersMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT(
		"/Game/MenuSystem/Menus/GameMenu/ServersMenu/WBP_ServerRow"
	));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UServersMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UServersMenu::JoinServer);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UServersMenu::Back);

	return true;
}

void UServersMenu::JoinServer() {
	if (Menu->GetMainMenuInterface() != nullptr) {
		if (!ensure(ServerRowClass != nullptr)) return;
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;
		ServerList->AddChild(ServerRow);
	}
}