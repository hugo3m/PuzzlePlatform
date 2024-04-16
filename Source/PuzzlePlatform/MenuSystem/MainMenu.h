// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */

class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;
class UPanelWidget;

USTRUCT()
struct FServerInfo
{
	GENERATED_BODY()

	FString Name;
	FString HostUsername;
	uint16 Players;
	uint16 MaxPlayers;
};

UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuCancelButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	TSubclassOf<UUserWidget> ServerRowClass;
	
	UFUNCTION()
	void OnStartHosting();

	UFUNCTION()
	void OnStartJoining();

	UFUNCTION()
	void OnOpenJoinMenu();

	UFUNCTION()
	void OnOpenHostMenu();

	UFUNCTION()
	void OnOpenMainMenu();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();

protected:
	virtual bool Initialize() override;

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerInfo> ServerInfos);

	void SelectIndex(uint32 Index);
	
};
