// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UItemUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FName ItemName;

public:
	void SetItemName(FName Name) { ItemName = Name; };
	
};
