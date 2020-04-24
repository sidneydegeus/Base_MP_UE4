// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExtendedPawn.generated.h"

UCLASS()
class BASE_MP_UE4_API AExtendedPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	class ABaseWeapon* Weapon;

protected:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AzimuthGimbal;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> UIClass;

	UPROPERTY()
	UUserWidget* UI;

	UPROPERTY(VisibleAnywhere)
	class UExitPawnComponent* ExitComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 MaxHealth = 100;

	int32 CurrentHealth;

private:
	float TotalDeltaPitch;



public:
	AExtendedPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ExitPawn();

	UFUNCTION(BlueprintCallable)
	void SetWeapon(ABaseWeapon* newWeapon) { Weapon = newWeapon; }

protected:
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetUI() { return UI; };

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void SetupUI();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void SetupUIEvent();
	virtual void SetupUIEvent_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void UpdateUIEvent();
	virtual void UpdateUIEvent_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_PossessedBy(APlayerController* PlayerController);
	virtual void Client_PossessedBy_Implementation(APlayerController* PlayerController);

	UFUNCTION(Client, Reliable)
	void Client_UnPossessed();
	virtual void Client_UnPossessed_Implementation();

	virtual void RotateAzimuthGimbal(float Delta);
	virtual void ElevateSpringArm(float Delta);

	class ABaseWeapon* GetWeapon() const;

private:
	void CreateCameraComponent();
	void UpdateUI();
};
