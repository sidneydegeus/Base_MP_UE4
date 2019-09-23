// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.generated.h"


UCLASS(Blueprintable)
class BASE_MP_UE4_API USpawnPoint : public USceneComponent
{
	GENERATED_BODY()

//Variables
protected:
	// Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(BlueprintReadWrite)
	AActor* SpawnedActor;

	//Functions
public:
	USpawnPoint();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetSpawnedActor() const { return SpawnedActor; }

protected:
	virtual void BeginPlay() override;

		
};
