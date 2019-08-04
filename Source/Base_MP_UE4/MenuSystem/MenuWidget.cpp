// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "Components/WidgetSwitcher.h"
#include "MenuSystem/SubMenuWidget.h"

void UMenuWidget::SetMainMenuInterface(IMainMenuInterface* ToSetMainMenuInterface) {
	MainMenuInterface = ToSetMainMenuInterface;
}

IMainMenuInterface* UMenuWidget::GetMainMenuInterface() {
	return this->MainMenuInterface;
}

void UMenuWidget::Setup() {
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDownWidget() {
	RemoveFromViewport();
	auto* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	auto* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly inputMode;
	PlayerController->SetInputMode(inputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
	TearDownWidget();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuWidget::OpenSubMenuWidget(TSubclassOf<USubMenuWidget> SubMenuClass) {
	if (!ensure(SubMenuClass != nullptr)) return;
	USubMenuWidget* SubMenu = CreateWidget<USubMenuWidget>(this, SubMenuClass);
	
	if (!ensure(SubMenu != nullptr)) return;
	SubMenu->SetMenu(this);

	if (ActiveSubMenuWidget == nullptr) {
		ActiveSubMenuWidget = SubMenu;
	} 
	else {
		// TODO: maybe an if statement around this later? In case a HUD will make use of this system
		// as the hud (such as an inventory) might not have back buttons, so swapping between
		// sub menu's will stop keep adding to the TArray / stack, which we do not want. 
		PreviousSubMenuWidgets.Push(ActiveSubMenuWidget);
		ActiveSubMenuWidget->RemoveFromParent();
		ActiveSubMenuWidget = SubMenu;
	}

	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->AddChild(SubMenu);
}

void UMenuWidget::OpenPreviousSubMenuWidget() {
	if (PreviousSubMenuWidgets.Num() < 1) return;
	USubMenuWidget* PrevSubMenu = PreviousSubMenuWidgets.Pop();
	if (!ensure(PrevSubMenu != nullptr)) return;

	if (ActiveSubMenuWidget != nullptr) {
		ActiveSubMenuWidget->RemoveFromParent();
		ActiveSubMenuWidget = PrevSubMenu;
	}

	if (!ensure(MenuSwitcher != nullptr)) return;
	MenuSwitcher->AddChild(PrevSubMenu);
}
