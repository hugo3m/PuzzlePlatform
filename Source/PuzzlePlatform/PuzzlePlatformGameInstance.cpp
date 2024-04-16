// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuWidget.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MainMenu.h"
#include "UObject/UnrealNames.h"


const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer) : UGameInstance(ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr )) return;
	MainMenuClass = MainMenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr )) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();
	const IOnlineSubsystem* OSubsystem = IOnlineSubsystem::Get();
	if (OSubsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem=%s"), *OSubsystem->GetSubsystemName().ToString());
		SessionInterface = OSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not find online subsystem"));
	}

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformGameInstance::OnNetworkFailure);
	}
}

void UPuzzlePlatformGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		
		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(SESSION_NAME);
		if (Session == nullptr)
		{
			this->CreateSession();	
		}
		else
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 Index)
{
	if(!SessionInterface.IsValid()) return;
	if(!SessionSearch.IsValid()) return;
	
	if(MainMenu != nullptr)
	{
		MainMenu->NativeDestruct();
	}
	
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	
}

void UPuzzlePlatformGameInstance::ToMainMenu()
{
	if(InGameMenu != nullptr)
	{
		InGameMenu->NativeDestruct();
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(
		"/Game/MenuSystem/MainMenu",
		 TRAVEL_Absolute
		);
}

void UPuzzlePlatformGameInstance::Exit()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	PlayerController->ConsoleCommand(TEXT("quit"), false);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuClass != nullptr )) return;
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if(!ensure(MainMenu != nullptr )) return;
	MainMenu->SetMenuInterface(this);
	MainMenu->Setup();
}

void UPuzzlePlatformGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr )) return;
	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if(!ensure(InGameMenu != nullptr )) return;
	InGameMenu->SetMenuInterface(this);
	InGameMenu->Setup();
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		// SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::StartSession()
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool isSuccess)
{
	if(!isSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session."));
		return;	
	} 
	if(MainMenu != nullptr)
	{
		MainMenu->NativeDestruct();
	}
	UWorld* World = GetWorld();
	if(!World) return;
	World->ServerTravel("/Game/PuzzlePlatform/Maps/Lobby?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool isSuccess)
{
	if (!isSuccess)
	{
		return;
	}
	this->CreateSession();
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool isSuccess)
{
	if (!isSuccess || !SessionSearch.IsValid() || MainMenu == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OnFindSessionsComplete"));
		return;
	}
	TArray<FServerInfo> ServerInfos;
	for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (SearchResult.IsValid())
		{
			FServerInfo ServerInfo;
			ServerInfo.Name = TEXT("Could not find name");
			ServerInfo.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerInfo.Players = ServerInfo.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			ServerInfo.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				ServerInfo.Name = ServerName;
			}
			
			ServerInfos.Add(ServerInfo);
		}
	}
	MainMenu->SetServerList(ServerInfos);
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	if(!SessionInterface.IsValid()) return;
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;

	FString Address;
	if(!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("OnJoinSessionComplete"));
		return;
	}
	
	PlayerController->ClientTravel(
		Address,
		TRAVEL_Absolute
		);
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = false;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		SessionSettings.NumPublicConnections = 3;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformGameInstance::OnNetworkFailure(UWorld* UWorld, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}
