// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/SubMenuWidget.h"
#include "ServersMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UServersMenu : public USubMenuWidget
{
	GENERATED_BODY()

//Variables
private:
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	TSubclassOf<class UUserWidget> ServerRowClass;

//Functions
public:
	UServersMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void JoinServer();
};
