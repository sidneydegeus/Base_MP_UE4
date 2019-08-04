// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "GameMenu.generated.h"

/**
 * GameMenu is an empty container that contain a MenuSwitcher which represents the current Menu State. 
 */
UCLASS()
class BASE_MP_UE4_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()

//Variables
private:
	TSubclassOf<class USubMenuWidget> MainMenuClass;

//Functions
public:
	UGameMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

};
