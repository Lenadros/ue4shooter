// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/ZWeapon.h"
#include "ZWeaponGun.generated.h"

USTRUCT()
struct FHitInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		int32 RandomSeed;

	UPROPERTY()
		FVector TraceStart;

	UPROPERTY()
		FVector TraceEnd;

	UPROPERTY()
		FVector Rotation;
};

/**
 * 
 */
UCLASS()
class PROJECTDURIAN_API AZWeaponGun : public AZWeapon
{
	GENERATED_BODY()

public:
	AZWeaponGun(const FObjectInitializer& ObjectInitializer);
	
	virtual void FireWeapon() override;

protected:

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerNotifyHit(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerNotifyMiss(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd);

	UFUNCTION()
		void OnRep_HitResult();

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		TSubclassOf<class AGunImpactEffect> ImpactClass;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_HitResult)
		FHitInfo HitInfo;

	void ProcessHit(const FHitResult& pHitRes, const FVector pTraceStart, const FVector pTraceEnd);

	void HitConfirmed(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd);

	void SpawnTrailFX(FVector pEndPoint);

	void SpawnImpactFX(const FHitResult pHitRes);

	void DamageActor(FHitResult pHitRes, FVector pShootDir);

	void SimulateEffects(FHitResult pHitRes, FVector pEndPoint);
};
