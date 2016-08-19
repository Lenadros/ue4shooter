// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ZWidgets/WPlayerInfo.h"
#include "ZWidgets/WInteractionText.h"
#include "ZWidgets/WGunStore.h"
#include "Character/ZPlayerController.h"
#include "ZHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AZHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	AZPlayerController* PC;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> WPlayerInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> WInteractionText;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> WGunStore;

	UWPlayerInfo* WPlayerInfoInst;

	UWInteractionText* WInteractionTextInst;

	UWGunStore* WGunStoreInst;

	void OpenGunStore();

	void CloseGunStore();
};
