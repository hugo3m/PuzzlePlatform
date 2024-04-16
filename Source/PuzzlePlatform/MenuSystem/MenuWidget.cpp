// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* Value)
{
	MenuInterface = Value;
}

void UMenuWidget::Setup()
{
	this->SetIsFocusable(true);
	this->AddToViewport();
	
	const UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void UMenuWidget::NativeDestruct()
{
	this->SetIsFocusable(false);
	this->RemoveFromParent();
	
	const UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	const FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
	Super::NativeDestruct();
}
