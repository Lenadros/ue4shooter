// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "Game/ZGameInstance.h"
#include "ZMainMenuWidget.h"

UZMainMenuWidget::UZMainMenuWidget(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("Constructed the main menu widget"));
}


void UZMainMenuWidget::HostServer(FName ServerName, int32 MaxPlayers, FString MapName)
{
	UE_LOG(LogTemp, Log, TEXT("Host server button clicked..."));

	UZGameInstance* gameInstance = Cast<UZGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		if (GEngine)
		{
			// Creating a local player where we can get the UserID from
			ULocalPlayer* const Player = GEngine->GetFirstGamePlayer(GetWorld());

			// Call our custom HostSession function. GameSessionName is a GameInstance variable
			gameInstance->HostGame(Player, ServerName, MaxPlayers, MapName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to get game instance"));
	}
}

void UZMainMenuWidget::SearchAllOnlineSessions()
{
	UE_LOG(LogTemp, Log, TEXT("Search sessions button clicked..."));

	UZGameInstance* gameInstance = Cast<UZGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		if (GEngine)
		{
			// Creating a local player where we can get the UserID from
			ULocalPlayer* const Player = GEngine->GetFirstGamePlayer(GetWorld());

			gameInstance->GetGameSession()->OnFindSessionsComplete().RemoveAll(this);
			OnFindSessionsCompleteDelegateHandle = gameInstance->GetGameSession()->OnFindSessionsComplete().AddUObject(this, &UZMainMenuWidget::OnFindSessionsComplete);

			// Call our custom HostSession function. GameSessionName is a GameInstance variable
			gameInstance->FindOnlineSessions(Player);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to get game instance"));
	}
}

void UZMainMenuWidget::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete in menu widget, bSuccess: %d"), bWasSuccessful));
	}

	UZGameInstance* gameInstance = Cast<UZGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		gameInstance->GetGameSession()->OnFindSessionsComplete().Remove(OnFindSessionsCompleteDelegateHandle);

		ClearServerList();

		auto SessionSearch = gameInstance->GetGameSession()->SessionSearch;
		if (SessionSearch->SearchResults.Num() > 0)
		{
			// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
			// This can be customized later on with your own classes to add more information that can be set and displayed
			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				int32 CurrentPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections
					+ SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPrivateConnections
					- SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections
					- SessionSearch->SearchResults[SearchIdx].Session.NumOpenPrivateConnections;
				int32 MaxPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections
					+ SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPrivateConnections;
				int32 PingInMs = SessionSearch->SearchResults[SearchIdx].PingInMs;

				AddServerListEntry(FString("Server hosted by: ") + SessionSearch->SearchResults[SearchIdx].Session.OwningUserName, CurrentPlayers, MaxPlayers, PingInMs, SearchIdx);
			}
		}
	}
}

void UZMainMenuWidget::OnQuitGameButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Quit game button clicked..."));
}