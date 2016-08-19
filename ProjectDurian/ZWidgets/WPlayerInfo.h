// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UWPlayerInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Player Info")
		float GetHealthRatio();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
		float GetShieldRatio();

	UFUNCTION(BlueprintCallable, Category = "Player Info")
		int32 GetCredits();
};
