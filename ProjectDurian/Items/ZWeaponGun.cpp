// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZWeaponGun.h"
#include "Character/ZPlayerController.h"
#include "Character/ZCharacter.h"
#include "Items/GunImpactEffect.h"

AZWeaponGun::AZWeaponGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AZWeaponGun::FireWeapon()
{
	//Initialize random stream generator with seed
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);

	//Get start and end points of trace and do a line trace
	const FVector StartTrace = GetCameraLoc();
	const FVector ShootDir = WeaponRandomStream.VRandCone(GetCameraRot(), 0.02f);
	const FVector EndTrace = StartTrace + ShootDir * 10000;
	const FHitResult HitRes = WeaponTrace(StartTrace, EndTrace);

	//Process the hit result
	ProcessHit(HitRes, StartTrace, EndTrace);
}

void AZWeaponGun::ProcessHit(const FHitResult& pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	//Check if owner is locally controlled
	if (mOwner && mOwner->IsLocallyControlled() && GetNetMode() == NM_Client)
	{
		//If hit object is an actor and on server, notify server of hit
		if (pHitRes.GetActor() && pHitRes.GetActor()->GetRemoteRole() == ROLE_Authority)
		{
			ServerNotifyHit(pHitRes, pTraceStart, pTraceEnd);
		}
		else if (pHitRes.GetActor() == NULL)
		{
			//If the hit is still blocking, notify server of hit, else notify of miss
			if (pHitRes.bBlockingHit)
			{
				ServerNotifyHit(pHitRes, pTraceStart, pTraceEnd);
			}
			else
			{
				ServerNotifyMiss(pHitRes, pTraceStart, pTraceEnd);
			}
		}
	}

	//Confirm hit if client
	HitConfirmed(pHitRes, pTraceStart, pTraceEnd);
}

bool AZWeaponGun::ServerNotifyHit_Validate(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	return true;
}

void AZWeaponGun::ServerNotifyHit_Implementation(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	if (Instigator && pHitRes.GetActor())
	{
		const FVector StartLoc = GetCameraLoc();
		const FVector AimDirection = (pHitRes.Location - StartLoc).GetSafeNormal();
		const float DotResult = Instigator->GetViewRotation().Vector() | AimDirection;

		if (DotResult > 0.9)
		{
			HitConfirmed(pHitRes, pTraceStart, pTraceEnd);
		}
	}
}

bool AZWeaponGun::ServerNotifyMiss_Validate(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	return true;
}

void AZWeaponGun::ServerNotifyMiss_Implementation(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	HitInfo.TraceStart = pTraceStart;
	HitInfo.TraceEnd = pTraceEnd;
	HitInfo.Rotation = GetCameraRot();

	if (GetNetMode() != NM_DedicatedServer)
	{
		const FVector EndPoint = GetCameraRot() * 10000;
	}
}

void AZWeaponGun::HitConfirmed(const FHitResult pHitRes, const FVector pTraceStart, const FVector pTraceEnd)
{
	//If an actor has been hit and this is the server, damage the actor
	if (pHitRes.GetActor() && GetNetMode() != NM_Client)
	{
		DamageActor(pHitRes, GetCameraRot());
	}

	if (Role == ROLE_Authority)
	{
		//If this is the server, replicate information about the hit to clients so they can simulate it
		HitInfo.TraceStart = pTraceStart;
		HitInfo.TraceEnd = pTraceEnd;
		HitInfo.Rotation = GetCameraRot();
	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		FVector EndPoint;
		if (pHitRes.GetActor())
		{
			EndPoint = pHitRes.ImpactPoint;
		}
		else
		{
			EndPoint = GetCameraRot() * 10000;
		}

		SpawnTrailFX(EndPoint);
		SpawnImpactFX(pHitRes);
	}
}

void AZWeaponGun::SpawnTrailFX(FVector pEndPoint)
{
	if (TrailFX)
	{
		const FVector StartPoint = GetMuzzleLocation();
		UParticleSystemComponent* TrailPS = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, StartPoint);
		if (TrailPS)
		{
			TrailPS->SetVectorParameter(FName("ShockBeamEnd"), pEndPoint);
		}
	}
}

void AZWeaponGun::SpawnImpactFX(const FHitResult pHitRes)
{
	if (ImpactClass && pHitRes.bBlockingHit)
	{
		FTransform const Transform(pHitRes.ImpactNormal.Rotation(), pHitRes.ImpactPoint);
		AGunImpactEffect* ImpactActor = GetWorld()->SpawnActorDeferred<AGunImpactEffect>(ImpactClass, Transform);
		if (ImpactActor)
		{
			ImpactActor->HitResult = pHitRes;
			UGameplayStatics::FinishSpawningActor(ImpactActor, FTransform(pHitRes.ImpactNormal.Rotation(), pHitRes.ImpactPoint));
		}
	}
}

void AZWeaponGun::DamageActor(FHitResult pHitRes, FVector pShootDir)
{
	FPointDamageEvent PointDmg;
	PointDmg.HitInfo = pHitRes;
	PointDmg.ShotDirection = pShootDir;
	PointDmg.Damage = 10;

	pHitRes.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, mOwner->Controller, this);
}

void AZWeaponGun::OnRep_HitResult()
{
	const FHitResult HitRes = WeaponTrace(HitInfo.TraceStart, HitInfo.TraceEnd);

	FVector EndPoint;
	if (HitRes.bBlockingHit)
	{
		EndPoint = HitRes.ImpactPoint;
	}
	else
	{
		EndPoint = HitInfo.Rotation * 10000;
	}

	SpawnTrailFX(EndPoint);
	SpawnImpactFX(HitRes);
}

void AZWeaponGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZWeaponGun, HitInfo, COND_SkipOwner);
}



