// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "CPlayerWeaponComponent.generated.h"

UCLASS(Blueprintable)
class USTALKER_API UCPlayerWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> RaiseAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> LowerAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> IdleAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> AimAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TObjectPtr<class UAnimationAsset> ReloadAnimation;
};
