// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PickUpComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class BASE_MP_UE4_API UPickUpComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditDefaultsOnly)
	//class UWidgetComponent* PickUpWidgetComponent;

public:
	UPickUpComponent(const FObjectInitializer& OI);
	virtual void BeginPlay() override;
};
