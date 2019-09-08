// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitPawnComponent.h"

#include "GameFramework/GameModeBase.h"

UExitPawnComponent::UExitPawnComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UExitPawnComponent::BeginPlay() {
	Super::BeginPlay();
}

//void UExitPawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)  {
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

void UExitPawnComponent::ExitPawn() {
	Server_ExitPawn();
}

void UExitPawnComponent::Server_ExitPawn_Implementation() {
	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner == nullptr) return;
	AController* Controller = Owner->GetController();
	if (Controller == nullptr) return;
	AGameModeBase* Mode = GetWorld()->GetAuthGameMode();
	if (Mode == nullptr) return;

	UClass* DefaultPawn = Mode->GetDefaultPawnClassForController(Controller);
	APawn* DefaultCharacter = Cast<APawn>(DefaultPawn);

	Controller->UnPossess();
	Controller->SetPawn(DefaultCharacter);
	//TODO: maybe make simulated proxy

	//TODO: RestartPlayerAtTransform... Use a socket or component for the transform location?
	//TODO: make use of something else in regards of AI? Not sure if restartplayer works on AI too.
	Mode->RestartPlayer(Controller);

}

bool UExitPawnComponent::Server_ExitPawn_Validate() {
	return true;
}

