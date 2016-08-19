// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WInteractionText.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UWInteractionText : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowMessage();

	void HideMessage();

	void SetMessage(FString pMessage);

	UFUNCTION(BlueprintCallable, Category = "HUD Text")
		FString GetMessage();

	FString mMessage;
};
