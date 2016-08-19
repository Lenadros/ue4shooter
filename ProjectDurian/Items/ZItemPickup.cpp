// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZItemPickup.h"
#include "Character/ZCharacter.h"


// Sets default values
AZItemPickup::AZItemPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USphereComponent* mPickupZone = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("PickupZone"));
	mPickupZone->InitSphereRadius(40.0f);
	mPickupZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mPickupZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = mPickupZone;

	mStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	mStaticMesh->AttachParent = RootComponent;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AZItemPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZItemPickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AZItemPickup::Pickup(AZPlayerController* pUser)
{
	//Do nothing here
}

void AZItemPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Cast<AZCharacter>(OtherActor))
	{
		AZPlayerController* PC = Cast<AZPlayerController>(Cast<AZCharacter>(OtherActor)->GetController());
		if (PC)
		{
			AZHUD* HUD = Cast<AZHUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->WInteractionTextInst->SetMessage("'E' to pick up " + mItemName);
				HUD->WInteractionTextInst->ShowMessage();
			}
		}
	}
}

void AZItemPickup::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (Cast<AZCharacter>(OtherActor))
	{
		AZPlayerController* PC = Cast<AZPlayerController>(Cast<AZCharacter>(OtherActor)->GetController());
		if (PC)
		{
			AZHUD* HUD = Cast<AZHUD>(PC->GetHUD());
			if (HUD)
				HUD->WInteractionTextInst->HideMessage();
		}
	}
}

