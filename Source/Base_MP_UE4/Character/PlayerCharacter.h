// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	class ABaseMP_PlayerController* PlayerController;

	FTimerHandle RespawnTimer;

// FUNCTIONS

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

protected:


	virtual void OnDeath() override;
	UFUNCTION()
	virtual void OnRespawn();
	
	virtual void LockTarget();

// Possession
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UFUNCTION(Client, Reliable)
	void Client_PossessedBy(ABaseMP_PlayerController* NewPlayerController);
	virtual void Client_PossessedBy_Implementation(ABaseMP_PlayerController* NewPlayerController);

	UFUNCTION(Client, Reliable)
	void Client_UnPossessed();
	virtual void Client_UnPossessed_Implementation();

	virtual void OnRep_EquippedWeapon() override;

	void Test();

private:
	void RemoveUI();
};
