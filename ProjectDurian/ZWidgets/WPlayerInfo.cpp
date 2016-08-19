// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "Character/ZCharacter.h"
#include "WPlayerInfo.h"

float UWPlayerInfo::GetHealthRatio()
{
	AZCharacter* Char = Cast<AZCharacter>(GetOwningPlayerPawn());
	if (Char)
	{
		return Char->GetCurrentHealth() / Char->GetMaxHealth();
	}
	return 0;
}

float UWPlayerInfo::GetShieldRatio()
{
	AZCharacter* Char = Cast<AZCharacter>(GetOwningPlayerPawn());
	if (Char)
	{
		return Char->GetCurrentShield() / Char->GetMaxShield();
	}
	return 0;
}

int32 UWPlayerInfo::GetCredits()
{
	AZCharacter* Char = Cast<AZCharacter>(GetOwningPlayerPawn());
	if (Char)
	{
		return Char->GetCredits();
	}
	return 0;
}




