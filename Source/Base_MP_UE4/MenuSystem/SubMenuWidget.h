// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API USubMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
//Variables
protected:
	class UMenuWidget* Menu;
	class USubMenuWidget* PreviousSubMenu;

//Functions
public:
	void SetMenu(class UMenuWidget* ToSetMenu);

protected:
	UFUNCTION()
		void Back();


};
