// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayer.h"

// Sets default values
ACPlayer::ACPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveAlong);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ACPlayer::MoveAlong);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ACPlayer::MoveAcross);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveAcross);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::AddControllerPitchInput);
}

void ACPlayer::MoveAlong(float Value)
{
	// Find out which way is "along" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ACPlayer::MoveAcross(float Value)
{
	// Find out which way is "across" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}
