// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
//#include "MenuSystem/GameMenu/ServersMenu/ServersMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MenuSystemGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UMenuSystemGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly)
		FString HostMap;

private:
	TSubclassOf<class UUserWidget> GameMenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;

	//UGLY, THINK OF A CLEANER WAY
	UPROPERTY()
		class UServersMenu* ServersMenu;


// Functions
public:
	UMenuSystemGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
		void LoadGameMenuWidget();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenuWidget();

	UFUNCTION(Exec)
		void Host(FString ServerName) override;

	UFUNCTION(Exec)
		void Join(uint32 Index) override;

	UFUNCTION(Exec)
		void QuitGame() override;


	void RefreshServerList(class UServersMenu* ToSetServersMenu) override;

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
};
