// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMPGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"



UBaseMPGameInstance::UBaseMPGameInstance(const FObjectInitializer & ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}

void UBaseMPGameInstance::Init()  {
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
}

void UBaseMPGameInstance::Host() {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/MultiplayerSessionMap?listen");
}

void UBaseMPGameInstance::Join(const FString& Address) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}