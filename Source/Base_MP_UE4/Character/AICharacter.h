// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWaypoint* NextWaypoint;

protected:
	virtual void OnDeath() override;
};
