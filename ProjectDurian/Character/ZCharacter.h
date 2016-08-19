// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Machines/ZGunStore.h"
#include "ZHUD.h"
#include "ZCharacterMovementComponent.h"
#include "Items/ZItemPickup.h"
#include "ZCharacter.generated.h"

UCLASS(Abstract)
class PROJECTDURIAN_API AZCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	// Camera for the player
	UCameraComponent* mCameraComp;

	// Player Information
	float mMaxHealth = 100;

	UPROPERTY(Replicated)
		float mHealth;

	float mMaxShield = 100;
	float mShield;
	
	int32 mCredits = 500;

public:
	virtual void PostInitializeComponents() override;

	// Sets default values for this character's properties
	AZCharacter(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Movement functions
	virtual void MoveForward(float val);

	virtual void MoveRight(float val);

	void OnCrouchToggle();

	void OnStartJump();

	void OnStopJump();

	void Landed(const FHitResult& Hit);

	void OnStartSprinting();

	void OnStopSprinting();

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		USkeletalMeshComponent* m1PMesh;

	//****************************************
	// Item/Object Interaction
	//****************************************

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerInteract();

	UFUNCTION(Client, Reliable)
		void MachineInteract();

	void Interact();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	TArray<AActor*> OverlappingActors;

	//****************************************
	// Weapons and Stuff
	//****************************************

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TSubclassOf<class AZWeapon> DefaultStartingWeapon;

	UPROPERTY(Transient, Replicated)
		TArray<class AZWeapon*> mInventory;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
		class AZWeapon* mCurrentWeapon;

	UPROPERTY(Transient, Replicated)
		bool bIsAiming;

	UPROPERTY(Transient, Replicated)
		bool bIsFiring;

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerEquipWeapon(class AZWeapon* pWeapon);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetAiming(bool pAiming);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetFiring(bool pFiring);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool IsAiming() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool IsFiring() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool GetIsEquipping();

	UFUNCTION()
		void OnRep_CurrentWeapon();

	FName WeaponJoint;

	void SetAiming(bool pAiming);

	void SetFiring(bool pFiring);

	void StartAiming();

	void StopAiming();

	void StartFire();

	void StopFire();

	void InitializeDefaultInventory();

	void AddWeapon(class AZWeapon* pWeapon);

	void EquipWeapon(class AZWeapon* pWeapon);

	void SetCurrentWeapon(class AZWeapon* pWeapon);

	void SwapWeapon();

	bool bIsEquipping;

	//****************************************
	// Damage
	//****************************************

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	//****************************************
	// Movement
	//****************************************

	UPROPERTY(Transient, Replicated)
		bool bWantsToRun;

	UPROPERTY(Transient, Replicated)
		bool bIsJumping;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float SprintingSpeedModifier;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsInitiatedJump() const;

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetIsJumping(bool NewJumping);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetSprinting(bool NewSprinting);
	
	UFUNCTION(BlueprintCallable, Category = Movement)
		bool IsSprinting() const;

	void SetSprinting(bool NewSprinting);

	float GetSprintingSpeedModifier() const;

	void SetIsJumping(bool NewJumping);

	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	// Get player information functions
	float GetCurrentHealth();
	float GetMaxHealth();
	float GetCurrentShield();
	float GetMaxShield();

	int32 GetCredits();
};
