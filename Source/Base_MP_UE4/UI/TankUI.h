// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TankUI.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UTankUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int Ammo) {
		AmmoText->SetText(FText::FromString(FString::FromInt(Ammo)));
	};
};
