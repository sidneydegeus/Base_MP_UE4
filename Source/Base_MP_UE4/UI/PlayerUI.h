// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponNameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* CrosshairImage;

public:
	void SetWeaponNameText(FName WeaponName) { 
		if (WeaponName == "None") WeaponNameText->SetText(FText::FromString(""));
		else WeaponNameText->SetText(FText::FromName(WeaponName)); 
	};
};
