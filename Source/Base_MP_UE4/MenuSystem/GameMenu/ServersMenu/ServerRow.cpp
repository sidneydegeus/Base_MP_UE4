// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"
#include "MenuSystem/GameMenu/ServersMenu/ServersMenu.h"

void UServerRow::Setup(UServersMenu * InParent, uint32 InIndex) {
	Parent = InParent;
	Index = InIndex;
	ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnServerRowButtonClicked);
	UE_LOG(LogTemp, Warning, TEXT("setup button server row"));
}

void UServerRow::OnServerRowButtonClicked() {
	if (!ensure(Parent != nullptr)) return;
	Parent->SelectIndex(Index);	
}