// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "ZPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AZPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	AZPlayerCameraManager(const class FObjectInitializer& ObjectInitializer);
	virtual void UpdateCamera(float DeltaTime) override;

public:
	// The default FOV
	float NormalFOV;
	
};
