// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/SubMenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMainMenu : public USubMenuWidget
{
	GENERATED_BODY()

//Variables
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ServersButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitGameButton;

	TSubclassOf<class USubMenuWidget> JoinMenuClass;
	TSubclassOf<class USubMenuWidget> ServersMenuClass;

//Functions
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

protected:
	virtual bool Initialize() override;


private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenServersMenu();

};
