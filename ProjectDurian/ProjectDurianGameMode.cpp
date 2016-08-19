// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ProjectDurianGameMode.h"
#include "Character/ZPlayerController.h"
#include "Online/ZGameSession.h"


AProjectDurianGameMode::AProjectDurianGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AZPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Blueprint'/Game/Blueprints/BP_ZCharacter'"));
	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDObject(TEXT("Blueprint'/Game/Blueprints/BP_ZHUD'"));
	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player pawn object was NULL!"));
	}

	if (PlayerHUDObject.Class != NULL)
	{
		HUDClass = PlayerHUDObject.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player HUD object was NULL!"));
        }

	PlayerControllerClass = AZPlayerController::StaticClass();
}

TSubclassOf< class AGameSession > AProjectDurianGameMode::GetGameSessionClass() const
{
	return AZGameSession::StaticClass();
}