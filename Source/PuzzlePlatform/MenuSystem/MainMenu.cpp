// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ServerRow.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

void UMainMenu::UpdateChildren()
{

	for (int32 Index = 0; Index != ServerList->GetChildrenCount(); ++Index)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(Index));
		if (Row != nullptr)
		{
			Row->IsSelected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == Index;
		}
	}
}

bool UMainMenu::Initialize()
{
	const bool Success = Super::Initialize();
	if(!Success) return false;

	if(!ensure(MainMenuHostButton != nullptr)) return false;
	MainMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::OnOpenHostMenu);

	if(!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartHosting);

	if(!ensure(MainMenuJoinButton != nullptr)) return false;
	MainMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnOpenJoinMenu);

	if(!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartJoining);

	if(!ensure(JoinMenuCancelButton != nullptr)) return false;
	JoinMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::OnOpenMainMenu);

	if(!ensure(HostMenuCancelButton != nullptr)) return false;
	HostMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::OnOpenMainMenu);
	

	return true;
	
}

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr )) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FServerInfo> ServerInfos)
{
	if(MenuInterface == nullptr) return;
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	
	for (int32 Index = 0; Index != ServerInfos.Num(); ++Index)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		Row->ServerName->SetText(FText::FromString(ServerInfos[Index].Name));
		Row->HostName->SetText(FText::FromString(ServerInfos[Index].HostUsername));
		FString PlayerInfoText = FString::Printf(TEXT("%d/%d"), ServerInfos[Index].Players, ServerInfos[Index].MaxPlayers);
		Row->PlayerInfo->SetText(FText::FromString(PlayerInfoText));
		Row->Setup(this, Index);
		ServerList->AddChild(Row);
	}
	
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::OnStartHosting()
{
	if(MenuInterface == nullptr) return;
	FString ServerName = ServerHostName->GetText().ToString();
	MenuInterface->Host(ServerName);
}

void UMainMenu::OnStartJoining()
{
	if(MenuInterface == nullptr) return;
	if(!SelectedIndex.IsSet()) return;
	MenuInterface->Join(SelectedIndex.GetValue());
}

void UMainMenu::OnOpenJoinMenu()
{
	if(!ensure(MenuSwitcher != nullptr)) return;
	if(!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OnOpenHostMenu()
{
	if(!ensure(MenuSwitcher != nullptr)) return;
	if(!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OnOpenMainMenu()
{
	if(!ensure(MenuSwitcher != nullptr)) return;
	if(!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}
