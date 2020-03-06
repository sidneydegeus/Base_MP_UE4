// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
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

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

public:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateUI();
	void UpdateUI_Implementation();

	void SetWeaponNameText(FName WeaponName) { 
		if (WeaponName == "None") {
			WeaponNameText->SetText(FText::FromString(""));
			AmmoText->SetText(FText::FromString(""));
		}
		else {
			WeaponNameText->SetText(FText::FromName(WeaponName));
		}
	};

	void DisplayCrosshair(bool Display) {
		if (Display) CrosshairImage->SetVisibility(ESlateVisibility::Visible);
		else  CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
	};

	void UpdateHealthBar(float Health) {
		HealthBar->SetPercent(Health);
	};

	void UpdateAmmo(int Ammo) {
		AmmoText->SetText(FText::FromString(FString::FromInt(Ammo)));
	};
};
