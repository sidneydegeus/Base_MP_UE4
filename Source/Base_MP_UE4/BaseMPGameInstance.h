// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseMPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UBaseMPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UBaseMPGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);
};
