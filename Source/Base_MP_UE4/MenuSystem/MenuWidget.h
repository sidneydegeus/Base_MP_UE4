// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuSystem/SubMenuWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

// Variables
protected:
	IMainMenuInterface* MainMenuInterface;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	USubMenuWidget* ActiveSubMenu;

private:
	TArray<USubMenuWidget*> PreviousSubMenuWidgets;
	USubMenuWidget* ActiveSubMenuWidget = nullptr;

// functions
public:
	void SetMainMenuInterface(IMainMenuInterface* ToSetMainMenuInterface);
	void Setup();

	void OpenSubMenuWidget(TSubclassOf<USubMenuWidget> SubMenuClass);
	void OpenPreviousSubMenuWidget();

	//maybe remove and make the ServerMenu (and others) UMenuWidgets instead
	IMainMenuInterface* GetMainMenuInterface();

protected:
	void TearDownWidget();

private:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

};
