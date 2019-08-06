// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "MenuSystem/MenuWidget.h"

UHostMenu::UHostMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	//
}

bool UHostMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UHostMenu::HostServer);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UHostMenu::Back);

	return true;
}

void UHostMenu::HostServer() {
	if (Menu->GetMenuInterface() != nullptr) {
		Menu->GetMenuInterface()->Host(ServerNameTextBox->GetText().ToString());
	}
}
