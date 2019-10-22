// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/SubMenuWidget.h"
#include "HostMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UHostMenu : public USubMenuWidget
{
	GENERATED_BODY()

		//Variables
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameTextBox;

	//Functions
public:
	UHostMenu(const FObjectInitializer & ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:

	UFUNCTION()
	void HostServer();
	
};
