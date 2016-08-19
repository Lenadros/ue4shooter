// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WGunStore.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UWGunStore : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Functions")
		void CloseStore();

	void MakeFocus();
};
