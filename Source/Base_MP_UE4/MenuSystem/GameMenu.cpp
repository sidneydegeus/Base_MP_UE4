// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"

#include "UObject/ConstructorHelpers.h"

UGameMenu::UGameMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr)) return;
	MainMenuClass = MainMenuBPClass.Class;
}

bool UGameMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(MainMenuClass != nullptr)) return false;
	OpenSubMenuWidget(MainMenuClass);

	return true;
}
