// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/SubMenuWidget.h"
#include "ServersMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString ServerName;
	FString HostUserName;
	uint16 CurrentPlayerCount;
	uint16 MaxPlayerCount;
};

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
	class UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* Loading;

	TSubclassOf<class UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

//Functions
public:
	UServersMenu(const FObjectInitializer& ObjectInitializer);
	virtual void SetMenu(class UMenuWidget* ToSetMenu) override;

	void SetServerList(TArray<FServerData> ServerNames);
	void SelectIndex(uint32 Index);


protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void RefreshServerList();

	void UpdateChildren();

};
