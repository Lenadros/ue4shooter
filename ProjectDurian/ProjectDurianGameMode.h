// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "ProjectDurianGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AProjectDurianGameMode : public AGameMode
{
	GENERATED_BODY()
	
	AProjectDurianGameMode(const FObjectInitializer& ObjectInitializer);

	TSubclassOf< class AGameSession > GetGameSessionClass() const;
	
	
};
