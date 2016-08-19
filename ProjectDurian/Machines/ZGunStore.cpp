// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "Character/ZHUD.h"
#include "ZGunStore.h"

void AZGunStore::UseMachine(AZPlayerController* pUser)
{
	Super::UseMachine(pUser);

	AZHUD* HUD = Cast<AZHUD>(pUser->GetHUD());
	if (HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Opening HUD"));
		HUD->OpenGunStore();
	}
}


