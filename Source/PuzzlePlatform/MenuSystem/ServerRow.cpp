// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* InParentMenu, uint32 InIndex)
{
	this->ParentMenu = InParentMenu;
	this->Index = InIndex;
	Button->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	if (ParentMenu != nullptr)
	{
		ParentMenu->SelectIndex(Index);
	}
}
