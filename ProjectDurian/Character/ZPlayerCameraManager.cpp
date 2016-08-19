// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"


AZPlayerCameraManager::AZPlayerCameraManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NormalFOV = 90.0f;

	// ViewPitchMin = -80.0f;
	// ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}


void AZPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	AZCharacter* MyPawn = PCOwner ? Cast<AZCharacter>(PCOwner->GetPawn()) : NULL;
	if (MyPawn)
	{
		const float TargetFOV = NormalFOV; // The FOV to use, do calculations to determine which FOV to use later when we have zoomable guns
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
		SetFOV(DefaultFOV);
	}

	Super::UpdateCamera(DeltaTime);
}