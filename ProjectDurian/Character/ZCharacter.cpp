// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZCharacter.h"
#include "Items/ZWeapon.h"



// Sets default values
AZCharacter::AZCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UZCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	// Adjust jump to make it less floaty
	MoveComp->GravityScale = 2.5f;
	MoveComp->JumpZVelocity = 620;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;

	// Enable crouching
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

	mCameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	// Position the camera a bit above the eyes
	mCameraComp->RelativeLocation = FVector(0, 0, 10 + BaseEyeHeight);
	// Allow the pawn to control rotation.
	mCameraComp->bUsePawnControlRotation = true;
	mCameraComp->AttachParent = GetCapsuleComponent();

	m1PMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("1st Person Mesh"));
	m1PMesh->AttachParent = mCameraComp;
	m1PMesh->bOnlyOwnerSee = true;
	m1PMesh->bOwnerNoSee = false;
	m1PMesh->bCastDynamicShadow = false;
	m1PMesh->bReceivesDecals = false;
	m1PMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	m1PMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m1PMesh->bChartDistanceFactor = false;
	m1PMesh->SetCollisionObjectType(ECC_Pawn);
	m1PMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m1PMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	SprintingSpeedModifier = 2.5f;
	bIsEquipping = false;

	// Set up player information
	mHealth = mMaxHealth;
	mShield = mMaxShield;

	//Assign name to joints
	WeaponJoint = TEXT("WeaponSocket");
}

// Called when the game starts or when spawned
void AZCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Using ZCharacter"));
	}

	InitializeDefaultInventory();
}

void AZCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeDefaultInventory();
}


// Called every frame
void AZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWantsToRun && !IsSprinting())
	{
		SetSprinting(true);
	}
}

// Called to bind functionality to input
void AZCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Movement
	InputComponent->BindAxis("MoveForward", this, &AZCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AZCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("Pitch", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAction("SprintHold", IE_Pressed, this, &AZCharacter::OnStartSprinting);
	InputComponent->BindAction("SprintHold", IE_Released, this, &AZCharacter::OnStopSprinting);

	InputComponent->BindAction("CrouchToggle", IE_Pressed, this, &AZCharacter::OnCrouchToggle);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AZCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AZCharacter::OnStopJump);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AZCharacter::Interact);
	InputComponent->BindAction("SwapWeapon", IE_Pressed, this, &AZCharacter::SwapWeapon);

	InputComponent->BindAction("Aim", IE_Pressed, this, &AZCharacter::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &AZCharacter::StopAiming);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AZCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AZCharacter::StopFire);
}


void AZCharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		// Limit pitch when walking or falling
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

		AddMovementInput(Direction, Val);
	}
}


void AZCharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}


void AZCharacter::OnStartJump()
{
	bPressedJump = true;

	SetIsJumping(true);
}


void AZCharacter::OnStopJump()
{
	bPressedJump = false;
}


bool AZCharacter::IsInitiatedJump() const
{
	return bIsJumping;
}


void AZCharacter::SetIsJumping(bool NewJumping)
{
	// Go to standing pose if trying to jump while crouched
	if (bIsCrouched && NewJumping)
	{
		UnCrouch();
	}
	else
	{
		bIsJumping = NewJumping;
	}

	if (Role < ROLE_Authority)
	{
		ServerSetIsJumping(NewJumping);
	}
}

void AZCharacter::ServerSetIsJumping_Implementation(bool NewJumping)
{
	SetIsJumping(NewJumping);
}

bool AZCharacter::ServerSetIsJumping_Validate(bool NewJumping)
{
	return true;
}

void AZCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetIsJumping(false);
}

void AZCharacter::SetSprinting(bool NewSprinting)
{
	bWantsToRun = NewSprinting;

	if (bIsCrouched)
		UnCrouch();

	// TODO: Stop weapon fire

	if (Role < ROLE_Authority)
	{
		ServerSetSprinting(NewSprinting);
	}
}


void AZCharacter::OnStartSprinting()
{
	SetSprinting(true);
}


void AZCharacter::OnStopSprinting()
{
	SetSprinting(false);
}


void AZCharacter::ServerSetSprinting_Implementation(bool NewSprinting)
{
	SetSprinting(NewSprinting);
}


bool AZCharacter::ServerSetSprinting_Validate(bool NewSprinting)
{
	return true;
}


bool AZCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
		return false;

	return bWantsToRun && !GetVelocity().IsZero()
		// Don't allow sprint while strafing sideways or standing still (1.0 is straight forward, -1.0 is backward while near 0 is sideways or standing still)
		&& (GetVelocity().GetSafeNormal2D() | GetActorRotation().Vector()) > 0.8; // Changing this value to 0.1 allows for diagonal sprinting. (holding W+A or W+D keys)
}


float AZCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}


void AZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Value is already updated locally, skip in replication step
	DOREPLIFETIME_CONDITION(AZCharacter, bWantsToRun, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AZCharacter, bIsJumping, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AZCharacter, bIsAiming, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AZCharacter, bIsFiring, COND_SkipOwner);

	DOREPLIFETIME(AZCharacter, mInventory);
	DOREPLIFETIME(AZCharacter, mCurrentWeapon);
	DOREPLIFETIME(AZCharacter, mHealth);
}

void AZCharacter::OnCrouchToggle()
{
	// If we are crouching then CanCrouch will return false. If we cannot crouch then calling Crouch() wont do anything
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AZCharacter::Interact()
{
	//Check if you are server, if not, call server function
	if (Role == ROLE_Authority)
	{
		//Get overlapping actors and determine if interacting with item or machine
		GetOverlappingActors(OverlappingActors);
		if (OverlappingActors.Num() != 0)
		{
			for (int i = 0; i < OverlappingActors.Num(); i++)
			{
				if (Cast<AZGunStore>(OverlappingActors[i]))
				{
					MachineInteract();
					break;
				}
				else if (Cast<AZItemPickup>(OverlappingActors[i]))
				{
					Cast<AZItemPickup>(OverlappingActors[i])->Pickup(Cast<AZPlayerController>(GetController()));
					break;
				}
			}
		}
	}
	else
	{
		ServerInteract();
	}
}

void AZCharacter::MachineInteract_Implementation()
{
	GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() != 0)
	{
		for (int i = 0; i < OverlappingActors.Num(); i++)
		{
			if (Cast<AZGunStore>(OverlappingActors[i]))
			{
				Cast<AZGunStore>(OverlappingActors[i])->UseMachine(Cast<AZPlayerController>(GetController()));
				break;
			}
		}
	}
}

void AZCharacter::ServerInteract_Implementation()
{
	Interact();
}

bool AZCharacter::ServerInteract_Validate()
{
	return true;
}

float AZCharacter::GetCurrentHealth()
{
	return mHealth;
}

float AZCharacter::GetMaxHealth()
{
	return mMaxHealth;
}

float AZCharacter::GetCurrentShield()
{
	return mShield;
}

float AZCharacter::GetMaxShield()
{
	return mMaxShield;
}

int32 AZCharacter::GetCredits()
{
	return mCredits;
}

void AZCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Actor Overlap Enter"));
}

void AZCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Actor Overlap Left"));
}

void AZCharacter::InitializeDefaultInventory()
{
	if (Role < ROLE_Authority)
		return;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AZWeapon* Weapon = GetWorld()->SpawnActor<AZWeapon>(DefaultStartingWeapon, SpawnParam);
	AddWeapon(Weapon);
}

void AZCharacter::AddWeapon(AZWeapon* pWeapon)
{
	if (pWeapon && Role == ROLE_Authority)
	{
		pWeapon->SetOwningPawn(this);
		mInventory.AddUnique(pWeapon);
	}
}

void AZCharacter::EquipWeapon(AZWeapon* pWeapon)
{
	if (pWeapon)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(pWeapon);
		}
		else
		{
			ServerEquipWeapon(pWeapon);
		}
	}
}

bool AZCharacter::ServerEquipWeapon_Validate(AZWeapon* pWeapon)
{
	return true;
}

void AZCharacter::ServerEquipWeapon_Implementation(AZWeapon* pWeapon)
{
	EquipWeapon(pWeapon);
}

void AZCharacter::SetCurrentWeapon(AZWeapon* pWeapon)
{
	mCurrentWeapon = pWeapon;

	if (pWeapon)
	{
		bIsEquipping = true;
		pWeapon->SetOwningPawn(this);
		pWeapon->EquipWeapon();
	}
}

void AZCharacter::OnRep_CurrentWeapon()
{
	SetCurrentWeapon(mCurrentWeapon);
}

void AZCharacter::SwapWeapon()
{
	EquipWeapon(mInventory[0]);
}

void AZCharacter::StartAiming()
{
	if (GetController())
	{
		if (IsSprinting())
			SetSprinting(false);

		SetAiming(true);
	}
}

void AZCharacter::StopAiming()
{
	SetAiming(false);
}

void AZCharacter::SetAiming(bool pAiming)
{
	bIsAiming = pAiming;

	if (Role < ROLE_Authority)
	{
		ServerSetAiming(pAiming);
	}
}

bool AZCharacter::ServerSetAiming_Validate(bool pAiming)
{
	return true;
}

void AZCharacter::ServerSetAiming_Implementation(bool pAiming)
{
	SetAiming(pAiming);
}

void AZCharacter::SetFiring(bool pFiring)
{
	bIsFiring = pFiring;

	if (Role < ROLE_Authority)
	{
		ServerSetFiring(pFiring);
	}
}

bool AZCharacter::ServerSetFiring_Validate(bool pFiring)
{
	return true;
}

void AZCharacter::ServerSetFiring_Implementation(bool pFiring)
{
	SetFiring(pFiring);
}

bool AZCharacter::IsAiming() const
{
	return bIsAiming;
}

bool AZCharacter::IsFiring() const
{
	return bIsFiring;
}

FRotator AZCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

bool AZCharacter::GetIsEquipping()
{
	return bIsEquipping;
}

float AZCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* AnimMesh;
	
	if (Controller && Controller->IsLocalPlayerController())
	{
		AnimMesh = m1PMesh;
	}
	else
	{
		AnimMesh = GetMesh();
	}

	if (AnimMontage && AnimMesh && AnimMesh->AnimScriptInstance && AnimMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage) == false)
	{
		return AnimMesh->AnimScriptInstance->Montage_Play(AnimMontage, 1.0f);
	}

	return 0.0f;
}

void AZCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent* AnimMesh;

	if (Controller && Controller->IsLocalPlayerController())
	{
		AnimMesh = m1PMesh;
	}
	else
	{
		AnimMesh = GetMesh();
	}

	if (AnimMontage && AnimMesh && AnimMesh->AnimScriptInstance && AnimMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		AnimMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOutTime);
	}
}

void AZCharacter::StartFire()
{
	if (GetController())
	{
		//If player is sprinting, stop the sprinting
		if (IsSprinting())
		{
			SetSprinting(false);
		}

		//If player has a weapon equiped, fire it
		if (mCurrentWeapon)
		{
			SetFiring(true);
			mCurrentWeapon->Fire();
		}
	}
}

void AZCharacter::StopFire()
{
	if (mCurrentWeapon)
	{
		SetFiring(false);
		mCurrentWeapon->StopFire();
	}
}

float AZCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		mHealth -= ActualDamage;
	}

	return ActualDamage;
}