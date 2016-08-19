// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Machines/ZMachine.h"
#include "Character/ZPlayerController.h"
#include "ZGunStore.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AZGunStore : public AZMachine
{
	GENERATED_BODY()
	
public:
	virtual void UseMachine(AZPlayerController* pUser) override;
};
