// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuSystemGameInstance.h"
#include "BaseMPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UBaseMPGameInstance : public UMenuSystemGameInstance
{
	GENERATED_BODY()
	
//Functions
public:
	virtual void Init() override;
};
