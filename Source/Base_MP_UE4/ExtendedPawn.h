// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExtendedPawn.generated.h"

UCLASS()
class BASE_MP_UE4_API AExtendedPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> UIClass;

	UPROPERTY()
	UUserWidget* UI;

protected:
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetUI() { return UI; };

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
};
