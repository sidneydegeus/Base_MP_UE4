// Fill out your copyright notice in the Description page of Project Settings.


#include "ServersMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/GameMenu/ServersMenu/ServerRow.h"

UServersMenu::UServersMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT(
		"/Game/MenuSystem/Menus/GameMenu/ServersMenu/WBP_ServerRow"	
	));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UServersMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UServersMenu::JoinServer);

	if (!ensure(BackButton != nullptr)) return false;
	RefreshButton->OnClicked.AddDynamic(this, &UServersMenu::RefreshServerList);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UServersMenu::Back);

	//RefreshServerList();
	return true;
}

void UServersMenu::SelectIndex(uint32 Index) {
	SelectedIndex = Index;
	UpdateChildren();
}

void UServersMenu::UpdateChildren() {
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i) {
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr) {
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UServersMenu::SetMenu(UMenuWidget * ToSetMenu) {
	Super::SetMenu(ToSetMenu);
	RefreshServerList();
}

void UServersMenu::JoinServer() {
	if (SelectedIndex.IsSet() && Menu->GetMenuInterface() != nullptr) {
		Menu->GetMenuInterface()->Join(SelectedIndex.GetValue());
	}
}

void UServersMenu::RefreshServerList() {
	if (Menu->GetMenuInterface() != nullptr) {
		Menu->GetMenuInterface()->RefreshServerList(this);
	}
}

void UServersMenu::SetServerList(TArray<FServerData> ServerDataList) {
	if (!ensure(ServerRowClass != nullptr)) return;
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& Data : ServerDataList) {
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;
		ServerRow->ServerName->SetText(FText::FromString(Data.ServerName));
		ServerRow->HostUserName->SetText(FText::FromString(Data.HostUserName));
		ServerRow->ServerPlayerCount->SetText(
			FText::FromString(FString::Printf(TEXT("%d/%d"), Data.CurrentPlayerCount, Data.MaxPlayerCount))
		);
		ServerRow->Setup(this, i);
		++i;

		ServerList->AddChild(ServerRow);
	}
}


