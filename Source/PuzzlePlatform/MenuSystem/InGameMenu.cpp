// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

void UInGameMenu::OnOpenMenu()
{
	if(MenuInterface == nullptr) return;
	MenuInterface->ToMainMenu();
}

void UInGameMenu::OnExit()
{
	if(MenuInterface == nullptr) return;
	MenuInterface->Exit();
}

void UInGameMenu::OnCancel()
{
	this->NativeDestruct();
}

bool UInGameMenu::Initialize()
{
	
	const bool Success = Super::Initialize();
	if(!Success) return false;

	if(!ensure(MenuButton != nullptr)) return false;
	MenuButton->OnClicked.AddDynamic(this, &UInGameMenu::OnOpenMenu);

	if(!ensure(ExitButton != nullptr)) return false;
	ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::OnExit);

	if(!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::OnCancel);

	return true;
}


