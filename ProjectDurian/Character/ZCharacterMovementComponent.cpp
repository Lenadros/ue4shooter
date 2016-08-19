// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZCharacterMovementComponent.h"
#include "ZCharacter.h"


float UZCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const AZCharacter* CharOwner = Cast<AZCharacter>(PawnOwner);
	if (CharOwner)
	{
		if (CharOwner->IsSprinting())
		{
			MaxSpeed *= CharOwner->GetSprintingSpeedModifier();
		}
	}

	return MaxSpeed;
}