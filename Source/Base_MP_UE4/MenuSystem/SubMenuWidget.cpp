// Fill out your copyright notice in the Description page of Project Settings.


#include "SubMenuWidget.h"
#include "MenuSystem/MenuWidget.h"

void USubMenuWidget::SetMenu(UMenuWidget* ToSetMenu) {
	Menu = ToSetMenu;
}

void USubMenuWidget::Back() {
	Menu->OpenPreviousSubMenuWidget();
}