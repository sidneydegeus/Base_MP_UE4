// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuInterface.h"
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
	IMenuInterface* MenuInterface;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY()
		USubMenuWidget* ActiveSubMenu;

private:
	UPROPERTY()
		TArray<USubMenuWidget*> PreviousSubMenuWidgets;

	UPROPERTY()
		USubMenuWidget* ActiveSubMenuWidget = nullptr;

// functions
public:
	void SetMenuInterface(IMenuInterface* ToSetMenuInterface);
	void Setup();

	void OpenSubMenuWidget(TSubclassOf<USubMenuWidget> SubMenuClass);
	void OpenPreviousSubMenuWidget();

	//maybe remove and make the ServerMenu (and others) UMenuWidgets instead
	IMenuInterface* GetMenuInterface();

protected:
	void TearDownWidget();

private:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

};
