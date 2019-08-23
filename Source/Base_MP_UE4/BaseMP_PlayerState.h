// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseMP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASE_MP_UE4_API ABaseMP_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	//UPROPERTY(Replicated)
	//	class APawn* ControlledPawn;

	UPROPERTY(ReplicatedUsing = OnRep_TestVal)
		FString TestVal;

public:
	void Test();
	FString GetTestVal() { return TestVal; };

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UFUNCTION()
		void OnRep_TestVal();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Test();
		void Server_Test_Implementation();
		bool Server_Test_Validate();
};
