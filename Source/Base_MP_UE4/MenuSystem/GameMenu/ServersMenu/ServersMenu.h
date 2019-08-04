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
		class UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	TSubclassOf<class UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

//Functions
public:
	UServersMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);
	virtual void SetMenu(class UMenuWidget* ToSetMenu) override;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void RefreshServerList();

};
