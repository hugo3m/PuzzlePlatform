// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"



class UUserWidget;
class UMenuWidget;
class UMainMenu;
/**
 * 
 */
UCLASS()
// double inheritance
class PUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance(
		const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	// UFUNCTION(Exec)
	virtual void Host(FString ServerName) override;

	// UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;
	
	virtual void ToMainMenu() override;

	virtual void Exit();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	void RefreshServerList() override;

	void StartSession();

private:
	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	UMainMenu* MainMenu;

	UMenuWidget* InGameMenu;

	IOnlineSessionPtr SessionInterface;

	void OnCreateSessionComplete(FName SessionName, bool isSuccess);

	void OnDestroySessionComplete(FName SessionName, bool isSuccess);

	void OnFindSessionsComplete(bool isSuccess);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;

	void OnNetworkFailure(UWorld* UWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
};
