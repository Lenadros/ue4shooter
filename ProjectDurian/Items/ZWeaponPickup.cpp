// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZWeaponPickup.h"

AZWeaponPickup::AZWeaponPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AZWeaponPickup::Pickup(AZPlayerController* pUser)
{
	Super::Pickup(pUser);

	Destroy(true);
}



