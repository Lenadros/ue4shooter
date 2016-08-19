// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Character/ZPlayerController.h"
#include "ZItemPickup.generated.h"

UCLASS()
class PROJECTDURIAN_API AZItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZItemPickup(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void Pickup(AZPlayerController* pUser);

	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FString mItemName;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* mStaticMesh;
	
};
