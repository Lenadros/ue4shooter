// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZGameInstance.h"


UZGameInstance::UZGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("Constructed game instance!"));
}

AZGameSession* UZGameInstance::GetGameSession() const
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AGameMode* const Game = World->GetAuthGameMode();
		if (Game)
		{
			return Cast<AZGameSession>(Game->GameSession);
		}
	}

	return nullptr;
}

// Starts playing a game as the host
bool UZGameInstance::HostGame(ULocalPlayer* LocalPlayer, FName SessionName, int32 MaxPlayers, FString MapName)
{
	AZGameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		// add callback delegate for completion
		// OnCreatePresenceSessionCompleteDelegateHandle = GameSession->OnCreatePresenceSessionComplete().AddUObject(this, &UZGameInstance::OnCreatePresenceSessionComplete);

		TravelURL = FString::Printf(TEXT("/Game/Maps/%s?listen"), *MapName);

		return GameSession->HostSession(LocalPlayer->GetPreferredUniqueNetId(), FName("ProjectDurian"), MaxPlayers, MapName, false, true);
	}

	return false;
}

// Find all online sessions
bool UZGameInstance::FindOnlineSessions(ULocalPlayer* LocalPlayer)
{
	AZGameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		GameSession->FindSessions(LocalPlayer->GetPreferredUniqueNetId(), FName("ProjectDurian"), false, true);
		return true;
	}

	return false;
}

bool UZGameInstance::JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults)
{
	AZGameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		GameSession->JoinSessionFromSearchIndex(LocalPlayer, SessionIndexInSearchResults);

		return true;
	}

	return false;
}