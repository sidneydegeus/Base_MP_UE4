// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
//Variables
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* InGameMenu_ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* InGameMenu_QuitToMainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* InGameMenu_ExitGameButton;

//Functions
protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void Resume();

	UFUNCTION()
	void QuitToMainMenu();

	UFUNCTION()
	void QuitGame();
};
