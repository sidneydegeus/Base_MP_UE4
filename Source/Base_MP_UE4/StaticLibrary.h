// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "StaticLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API UStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	static FString GetNetRoleEnumAsString(ENetRole EnumValue);
};
