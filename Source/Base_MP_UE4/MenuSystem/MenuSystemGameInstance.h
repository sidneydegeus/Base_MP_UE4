// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
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

// Functions
public:
	UMenuSystemGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
		void LoadGameMenuWidget();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenuWidget();

	UFUNCTION(Exec)
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& Address) override;

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);

	void CreateSession();
};
