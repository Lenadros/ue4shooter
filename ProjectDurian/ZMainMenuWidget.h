// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Online.h"
#include "ZMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UZMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UZMainMenuWidget(const class FObjectInitializer& ObjectInitializer);

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

public:

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void HostServer(FName ServerName, int32 MaxPlayers, FString MapName);

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void SearchAllOnlineSessions();

	UFUNCTION(BlueprintCallable, Category = "Game")
		void OnQuitGameButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Online|Sessions")
		void ClearServerList();

	UFUNCTION(BlueprintImplementableEvent, Category = "Online|Sessions")
		void AddServerListEntry(const FString & ServerName, int32 CurrentPlayers, int32 MaxPlayers, int32 Ping, int32 SearchResultIndex);
};
