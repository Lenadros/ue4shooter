// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "ZMachine.h"
#include "Character/ZCharacter.h"
#include "Character/ZHUD.h"

// Sets default values
AZMachine::AZMachine(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* mTriggerField = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("TriggerField"));
	mTriggerField->InitSphereRadius(250.0f);
	mTriggerField->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mTriggerField->SetCollisionResponseToAllChannels(ECR_Ignore);
	mTriggerField->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = mTriggerField;

}

// Called when the game starts or when spawned
void AZMachine::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZMachine::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AZMachine::UseMachine(AZPlayerController* pUser)
{
	UE_LOG(LogTemp, Warning, TEXT("Machine has been accessed by a character!"));
}

void AZMachine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Cast<AZCharacter>(OtherActor))
	{
		AZPlayerController* PC = Cast<AZPlayerController>(Cast<AZCharacter>(OtherActor)->GetController());
		if (PC)
		{
			AZHUD* HUD = Cast<AZHUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->WInteractionTextInst->SetMessage(mUseMessage);
				HUD->WInteractionTextInst->ShowMessage();
			}
		}
	}
}

void AZMachine::NotifyActorEndOverlap(AActor* OtherActor)
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

