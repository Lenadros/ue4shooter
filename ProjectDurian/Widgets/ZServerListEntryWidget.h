// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ZServerListEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UZServerListEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UZServerListEntryWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, Category = "Online")
		void SetSessionData(int32 SessionSearchResultsIndex);

	UFUNCTION(BlueprintCallable, Category = "Online")
		void JoinGame();
	
protected:
	int32 mSessionSearchResultsIndex;
};
