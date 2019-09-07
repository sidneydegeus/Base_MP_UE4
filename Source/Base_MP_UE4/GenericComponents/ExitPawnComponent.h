// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExitPawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASE_MP_UE4_API UExitPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExitPawnComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ExitPawn();


protected:

	virtual void BeginPlay() override;

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ExitPawn();
		void Server_ExitPawn_Implementation();
		bool Server_ExitPawn_Validate();
		
};
