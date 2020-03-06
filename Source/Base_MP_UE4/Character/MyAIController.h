// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWandering;

private:
	UPROPERTY()
	class AAICharacter* PossessedCharacter;

	UPROPERTY()
	class ABaseCharacter* DetectedCharacter;

public:
	AMyAIController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	virtual void OnPossess(APawn* Pawn) override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Wander();
	void Wander_Implementation() {};

	UFUNCTION(BlueprintCallable)
	bool HasDetectedPlayer() { return DetectedCharacter; };

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawns);


};
