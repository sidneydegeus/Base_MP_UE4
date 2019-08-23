// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// How far ahead of the player can we reach in cm
	float Reach = 150.f;
	// Returns current start of reach line
	FVector GetReachLineStart();
	// Returns current end of reach line
	FVector GetReachLineEnd();

public:	
	UInteractionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact();
protected:


public:	


		
};
