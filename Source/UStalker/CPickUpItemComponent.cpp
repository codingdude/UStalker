// Fill out your copyright notice in the Description page of Project Settings.

#include "CPickUpItemComponent.h"
#include "CPlayerWeaponComponent.h"
#include "CPlayer.h"


void UCPickUpItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UCPickUpItemComponent::OnSphereBeginOverlap);
}

void UCPickUpItemComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	if (Player != nullptr && ComponentClass != nullptr)
	{
		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);

		// Create a first person mesh component for the owning player.
		USkeletalMeshComponent* WeaponComponent =
			NewObject<USkeletalMeshComponent>(Player, ComponentClass);
		check(WeaponComponent != nullptr);

		Player->SetupAttachableItemComponent(WeaponComponent);

		// Remove pick-up object from the scene
		GetOwner()->Destroy();
	}
}
