// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "ZCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API UZCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual float GetMaxSpeed() const override;
	
	
};
