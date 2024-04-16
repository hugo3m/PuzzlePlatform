// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberLoggedPlayers;
	if(NumberLoggedPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(StartGameTimer, this, &ALobbyGameMode::StartGame, 5.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberLoggedPlayers;
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	UPuzzlePlatformGameInstance* PPGameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (PPGameInstance != nullptr)
	{
		PPGameInstance->StartSession();
	}
	World->ServerTravel("/Game/PuzzlePlatform/Maps/Game?listen");
}
