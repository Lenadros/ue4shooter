// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZWeapon.h"
#include "Character/ZPlayerController.h"
#include "Character/ZCharacter.h"


// Sets default values
AZWeapon::AZWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m1PMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Weapon Mesh 1P"));
	m1PMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	m1PMesh->bChartDistanceFactor = false;
	m1PMesh->bReceivesDecals = false;
	m1PMesh->CastShadow = false;
	m1PMesh->SetCollisionObjectType(ECC_WorldDynamic);
	m1PMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m1PMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = m1PMesh;

	m3PMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Weapon Mesh 3P"));
	m3PMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	m3PMesh->bChartDistanceFactor = true;
	m3PMesh->bReceivesDecals = false;
	m3PMesh->CastShadow = true;
	m3PMesh->SetCollisionObjectType(ECC_WorldDynamic);
	m3PMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m3PMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	m3PMesh->AttachParent = m1PMesh;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);

	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	mNumShots = 0;
	bHasAmmo = true;

	mMuzzleJoint = TEXT("MuzzleFlashSocket");
}

void AZWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetachGunMesh();
}

// Called when the game starts or when spawned
void AZWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AZWeapon::SetOwningPawn(AZCharacter* pOwner)
{
	if (mOwner != pOwner)
	{
		Instigator = pOwner;
		mOwner = pOwner;
		SetOwner(pOwner);
	}
}

void AZWeapon::EquipWeapon()
{
	if (mOwner)
	{
		DetachGunMesh();

		float AnimDuration;
		if (mOwner->Controller && mOwner->Controller->IsLocalPlayerController())
		{
			AnimDuration = mOwner->PlayAnimMontage(m1PEquipAnim);
		}
		else
		{
			AnimDuration = mOwner->PlayAnimMontage(m3PEquipAnim);
		}

		GetWorldTimerManager().SetTimer(TimerHandle_EquipAnim, this, &AZWeapon::AttachGunMesh, AnimDuration / 2, false);
	}
}

void AZWeapon::AttachGunMesh()
{
	if (mOwner)
	{
		if (mOwner->Controller && mOwner->Controller->IsLocalPlayerController())
		{
			USkeletalMeshComponent* FirstPersonMesh = mOwner->m1PMesh;
			m1PMesh->SetHiddenInGame(false);
			m1PMesh->AttachTo(FirstPersonMesh, mOwner->WeaponJoint);
		}
		else
		{
			USkeletalMeshComponent* ThirdPersonMesh = mOwner->GetMesh();
			m3PMesh->SetHiddenInGame(false);
			m3PMesh->AttachTo(ThirdPersonMesh, mOwner->WeaponJoint);
		}
	}
}

void AZWeapon::DetachGunMesh()
{
	m1PMesh->DetachFromParent();
	m1PMesh->SetHiddenInGame(true);

	m3PMesh->DetachFromParent();
	m3PMesh->SetHiddenInGame(true);
}

void AZWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZWeapon, mNumShots, COND_SkipOwner);

	DOREPLIFETIME(AZWeapon, mOwner);
}

void AZWeapon::OnRep_MyPawn()
{
	if (mOwner)
		SetOwningPawn(mOwner);
}

void AZWeapon::EquipEnd()
{
	if (mOwner)
		mOwner->bIsEquipping = false;
}

void AZWeapon::Fire()
{
	//Notify server of fire
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	FireShot();
}

void AZWeapon::StopFire()
{
	//Notify sever of stop fire
	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}

	StopSimulateShot();
}

bool AZWeapon::ServerFire_Validate()
{
	return true;
}

void AZWeapon::ServerFire_Implementation()
{
	//Call fire on server and replicate to clients that fire has been shot
	Fire();
	mNumShots++;
}

bool AZWeapon::ServerStopFire_Validate()
{
	return true;
}

void AZWeapon::ServerStopFire_Implementation()
{
	//Replicate to clients the actor has stopped firing
	mNumShots = 0;
	StopFire();
}

void AZWeapon::FireShot()
{
	//Check if the gun has ammo or not
	if (bHasAmmo)
	{
		//If function is called on clients (or not dedicated server) simulate the shot through animations
		if (GetNetMode() != NM_DedicatedServer)
		{
			SimulateShot();
		}

		//If the owner is local, start the ray trace
		if (mOwner && mOwner->IsLocallyControlled())
		{
			FireWeapon();
		}
	}

	if (mOwner && mOwner->IsLocallyControlled())
	{
		//If the owner is local, repeat the fire shot protocol (Automatic Weapon)
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AZWeapon::Fire, 0.2f, false);
	}
}

void AZWeapon::SimulateShot()
{
	if (mOwner && mOwner->IsLocallyControlled())
	{
		//Play first person fire animation and spawn first person muzzle flash if locally controlled
		mOwner->PlayAnimMontage(mFireAnim);
		if (m1PMuzzleFlash == NULL)
		{
			m1PMuzzleFlash = UGameplayStatics::SpawnEmitterAttached(mMuzzleFlash, m1PMesh, mMuzzleJoint);
			m1PMuzzleFlash->bOwnerNoSee = false;
			m1PMuzzleFlash->bOnlyOwnerSee = true;
		}
	}
	else
	{
		//Spawn third person muzzle flash if locally controlled
		if (m3PMuzzleFlash == NULL)
		{
			m3PMuzzleFlash = UGameplayStatics::SpawnEmitterAttached(mMuzzleFlash, m3PMesh, mMuzzleJoint);
			m3PMuzzleFlash->bOwnerNoSee = true;
			m3PMuzzleFlash->bOnlyOwnerSee = false;
		}
	}
}

void AZWeapon::StopSimulateShot()
{
	//Stop any firing animations, stop refire timer, and remove all particle effects
	mOwner->StopAnimMontage(mFireAnim);
	//mOwner->StopAnimMontage(RifleADSFireAnim);
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);

	if (m1PMuzzleFlash != NULL)
	{
		m1PMuzzleFlash->DeactivateSystem();
		m1PMuzzleFlash = NULL;
	}
	if (m3PMuzzleFlash != NULL)
	{
		m3PMuzzleFlash->DeactivateSystem();
		m3PMuzzleFlash = NULL;
	}
}

void AZWeapon::OnRep_NumShots()
{
	if (mNumShots > 0)
	{
		//Simulate shot on client if shot counter is above 0
		SimulateShot();
	}
	else
	{
		//Stop shot simulation if shot counter is 0
		StopSimulateShot();
	}
}

bool AZWeapon::ServerFireShot_Validate()
{
	return true;
}

void AZWeapon::ServerFireShot_Implementation()
{
	FireShot();
	mNumShots++;
}

FVector AZWeapon::GetCameraLoc()
{
	AZPlayerController* PC = Cast<AZPlayerController>(mOwner->GetController());
	FVector FinalLoc = FVector::ZeroVector;

	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FinalLoc = CamLoc;
	}

	return FinalLoc;
}

FVector AZWeapon::GetCameraRot()
{
	AZPlayerController* PC = NULL;
	if (mOwner)
	{
		PC = Cast<AZPlayerController>(mOwner->GetController());
	}
	FVector FinalRot = FVector::ZeroVector;

	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FinalRot = CamRot.Vector();
	}

	return FinalRot;
}

FHitResult AZWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace)
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1, TraceParams);

	return Hit;
}

FVector AZWeapon::GetMuzzleLocation()
{
	if (mOwner->Controller && mOwner->Controller->IsLocalPlayerController())
	{
		return m1PMesh->GetSocketLocation(mMuzzleJoint);
	}
	else
	{
		return m3PMesh->GetSocketLocation(mMuzzleJoint);
	}
}


