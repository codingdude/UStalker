// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class USTALKER_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Handles input for moving forward
	UFUNCTION()
	void MoveAlong(float Value);

	// Handles input for moving left
	UFUNCTION()
	void MoveAcross(float Value);

	// Handles input on take weapon on/off
	UFUNCTION()
	void SwitchWeapon();

	// Handles input on aim weapon
	UFUNCTION()
	void AimWeapon();

	// Handles input on idle weapon
	UFUNCTION()
	void IdleWeapon();

	// Handles input on fire weapon
	UFUNCTION()
	void FireWeapon();

	// Handles input on reload weapon
	UFUNCTION()
	void ReloadWeapon();


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<class UCPlayerWeaponComponent> WeaponComponent;
};
