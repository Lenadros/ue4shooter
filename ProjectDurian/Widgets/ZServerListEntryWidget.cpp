// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "../Game/ZGameInstance.h"
#include "ZServerListEntryWidget.h"


UZServerListEntryWidget::UZServerListEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UZServerListEntryWidget::SetSessionData(int32 SessionSearchResultsIndex)
{
	mSessionSearchResultsIndex = SessionSearchResultsIndex;
}

void UZServerListEntryWidget::JoinGame()
{
	// Call the join function from the game instance and pass in the session name and search result index
	UE_LOG(LogTemp, Log, TEXT("Joining the game"));

	UZGameInstance* gameInstance = Cast<UZGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		if (GEngine)
		{
			// Creating a local player where we can get the UserID from
			ULocalPlayer* const Player = GEngine->GetFirstGamePlayer(GetWorld());

			// Call our custom HostSession function. GameSessionName is a GameInstance variable
			gameInstance->JoinSession(Player, mSessionSearchResultsIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to get game instance"));
	}
}