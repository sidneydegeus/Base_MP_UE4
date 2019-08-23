// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMP_PlayerState.h"

#include "Net/UnrealNetwork.h"

void ABaseMP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseMP_PlayerState, TestVal);
	//DOREPLIFETIME(ABaseMP_PlayerState. ControlledPawn);
}

void ABaseMP_PlayerState::Test() {
	//if (Role < ROLE_Authority) {
		UE_LOG(LogTemp, Warning, TEXT("attempting setting state"));
		Server_Test_Implementation();
	//}
}

void ABaseMP_PlayerState::OnRep_TestVal() {
	UE_LOG(LogTemp, Warning, TEXT("after setting state: %s"), *GetTestVal());
}

void ABaseMP_PlayerState::Server_Test_Implementation() {
	TestVal = "Value replicated";
}

bool ABaseMP_PlayerState::Server_Test_Validate() {
	return true;
}