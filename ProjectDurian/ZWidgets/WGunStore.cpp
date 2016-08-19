// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "Character/ZPlayerController.h"
#include "Character/ZHUD.h"
#include "WGunStore.h"

void UWGunStore::CloseStore()
{
	AZPlayerController* PC = Cast<AZPlayerController>(GetOwningPlayer());
	if (PC)
	{
		Cast<AZHUD>(PC->GetHUD())->CloseGunStore();
	}
}

void UWGunStore::MakeFocus()
{

}


