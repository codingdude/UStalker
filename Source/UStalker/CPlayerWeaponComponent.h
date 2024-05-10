// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "CPlayerWeaponComponent.generated.h"

UCLASS(Blueprintable)
class USTALKER_API UCPlayerWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

private:
	enum EWeaponState
	{
		WS_None,
		WS_Raise,
		WS_Lower,
		WS_Idle,
		WS_Aim,
		WS_Fire,
		WS_Reload
	};

	void SwitchToState(EWeaponState NewState);

public:
	UCPlayerWeaponComponent();

	void RaiseWeapon() { SwitchToState(WS_Raise); }
	void LowerWeapon() { SwitchToState(WS_Lower); }
	void IdleWeapon() { SwitchToState(WS_Idle); }
	void AimWeapon() { SwitchToState(WS_Aim); }
	void FireWeapon() { SwitchToState(WS_Fire); }
	void ReloadWeapon() { SwitchToState(WS_Reload); }

	bool IsWeaponRaised() { return CurrentState != WS_None; }

public:
	UFUNCTION()
	void OnAnimationEndedNotify(const FString& Name, UAnimSequenceBase* Animation);

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

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

private:
	FAnimNotifyEvent AnimationEndedNotifyEvent;

	EWeaponState CurrentState;

	bool bTransitionInProgress;
};
