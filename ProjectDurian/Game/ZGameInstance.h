// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Online.h"
#include "Delegate.h"
#include "../Online/ZGameSession.h"
#include "ZGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UZGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UZGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, Category = "Online")
		AZGameSession* GetGameSession() const;

	bool HostGame(ULocalPlayer* LocalPlayer, FName SessionName, int32 MaxPlayers, FString MapName);
	bool FindOnlineSessions(ULocalPlayer* LocalPlayer);
	bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults);

	FString TravelURL;
};
