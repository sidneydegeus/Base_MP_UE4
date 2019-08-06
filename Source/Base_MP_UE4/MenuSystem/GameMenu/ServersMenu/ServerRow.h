// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

//Variables
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerPlayerCount;


	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ServerRowButton;

	UPROPERTY()
		class UServersMenu* Parent;

	uint32 Index;

//Functions
public:
	void Setup(UServersMenu* InParent, uint32 InIndex);

private:
	UFUNCTION()
		void OnServerRowButtonClicked();
};
