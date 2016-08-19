// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZPlayerController.h"
#include "ZPlayerCameraManager.h"
#include "Online.h"
#include "OnlineAchievementsInterface.h"
#include "OnlineEventsInterface.h"
#include "OnlineIdentityInterface.h"
#include "OnlineSessionInterface.h"

AZPlayerController::AZPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AZPlayerCameraManager::StaticClass();
}


// Called when the game starts or when spawned
void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();
}