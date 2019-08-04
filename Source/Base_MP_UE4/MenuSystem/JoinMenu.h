// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/SubMenuWidget.h"
#include "JoinMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UJoinMenu : public USubMenuWidget
{
	GENERATED_BODY()

//Variables
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressTextBox;

//Functions
public:
	UJoinMenu(const FObjectInitializer & ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void JoinIP();
};
