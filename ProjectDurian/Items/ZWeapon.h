// Fill out your copyright notice in the Description page of Project Settings./

#pragma once

#include "GameFramework/Actor.h"
#include "ZWeapon.generated.h"

class AZCharacter;

UCLASS(abstract)
class PROJECTDURIAN_API AZWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZWeapon(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void EquipWeapon();

	void Fire();

	void StopFire();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStopFire();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		AZCharacter* mOwner;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_NumShots)
		int32 mNumShots;

	void SetOwningPawn(AZCharacter* pOwner);

protected:

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* m3PMesh;

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* m1PMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* m3PEquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* m1PEquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* mFireAnim;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* mMuzzleFlash;

	UPROPERTY(Transient)
		UParticleSystemComponent* m1PMuzzleFlash;

	UPROPERTY(Transient)
		UParticleSystemComponent* m3PMuzzleFlash;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFireShot();

	UFUNCTION()
		void OnRep_MyPawn();

	UFUNCTION()
		void OnRep_NumShots();

	virtual void FireWeapon() PURE_VIRTUAL(AShooterWeapon::Fire,)

	void AttachGunMesh();

	void DetachGunMesh();

	void EquipEnd();

	void FireShot();

	void SimulateShot();

	void StopSimulateShot();

	bool bHasAmmo;

	FVector GetMuzzleLocation();

	FVector GetCameraLoc();

	FVector GetCameraRot();

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace);

	FTimerHandle TimerHandle_EquipAnim;

	FTimerHandle TimerHandle_Fire;

	FName mMuzzleJoint;
	
};
