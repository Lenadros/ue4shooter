// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZHUD.h"

void AZHUD::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AZPlayerController>(GetOwningPlayerController());
	if (PC)
	{
		//Create widget instances from blueprints
		WPlayerInfoInst = CreateWidget<UWPlayerInfo>(PC, WPlayerInfo);
		WInteractionTextInst = CreateWidget<UWInteractionText>(PC, WInteractionText);
		WGunStoreInst = CreateWidget<UWGunStore>(PC, WGunStore);

		//Add widget instances to the viewport
		WPlayerInfoInst->AddToViewport();
		WInteractionTextInst->AddToViewport();
		WGunStoreInst->AddToViewport();

		//Hide any widgets that should not be visible
		WInteractionTextInst->SetVisibility(ESlateVisibility::Hidden);
		WGunStoreInst->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AZHUD::OpenGunStore()
{
	WGunStoreInst->SetVisibility(ESlateVisibility::Visible);
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	PC->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());
}

void AZHUD::CloseGunStore()
{
	WGunStoreInst->SetVisibility(ESlateVisibility::Hidden);
	PC->bShowMouseCursor = false;
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;
	PC->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
}


