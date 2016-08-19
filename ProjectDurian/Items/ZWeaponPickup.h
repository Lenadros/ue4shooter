// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/ZItemPickup.h"
#include "Character/ZPlayerController.h"
#include "ZWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AZWeaponPickup : public AZItemPickup
{
	GENERATED_BODY()
	
public:

	AZWeaponPickup(const FObjectInitializer& ObjectInitializer);

	void Pickup(AZPlayerController *pUser) override;
	
	
};
