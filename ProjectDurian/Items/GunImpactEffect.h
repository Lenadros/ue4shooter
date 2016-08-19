// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GunImpactEffect.generated.h"

UCLASS(Abstract, Blueprintable)
class PROJECTDURIAN_API AGunImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunImpactEffect(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
		UParticleSystem* DefaultPS;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
		UParticleSystem* BloodPS;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		UMaterial* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float Lifespan;

	UPROPERTY(BlueprintReadOnly, Category = Surface)
		FHitResult HitResult;

protected:
	
	UParticleSystem* GetImpactEffect(TEnumAsByte<EPhysicalSurface> SurfaceType) const;

	USoundCue* GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
	
};
