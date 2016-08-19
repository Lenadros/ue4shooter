// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "GunImpactEffect.h"


// Sets default values
AGunImpactEffect::AGunImpactEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoDestroyWhenFinished = true;
}

void AGunImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* PhysicalMaterial = HitResult.PhysMaterial.Get();
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMaterial);

	UParticleSystem* ImpactEffect = GetImpactEffect(SurfaceType);
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
	}

	if (DecalMaterial)
	{
		FRotator RandomRotation = HitResult.ImpactNormal.Rotation();
		RandomRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(DecalSize, DecalSize, 1.0f), HitResult.Component.Get(), HitResult.BoneName, HitResult.ImpactPoint, RandomRotation, EAttachLocation::KeepWorldPosition, Lifespan);
	}
}

// Called when the game starts or when spawned
void AGunImpactEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGunImpactEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

UParticleSystem* AGunImpactEffect::GetImpactEffect(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem* FX = NULL;

	switch (SurfaceType)
	{
		case EPhysicalSurface::SurfaceType1:
			FX = BloodPS;
			break;
		default:
			FX = DefaultPS;
			break;
	}

	return FX;
}

