// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerWeaponComponent.h"
#include "CAnimNotify.h"

#include "Animation/AnimSingleNodeInstance.h"
#include "Components/AudioComponent.h"

UCPlayerWeaponComponent::UCPlayerWeaponComponent()
	: Super()
	, CurrentState(WS_None)
{
}

void UCPlayerWeaponComponent::SwitchWeapon()
{
	if (CurrentState == WS_None)
	{
		RaiseWeapon();
	}
	else
	{
		LowerWeapon();
	}
}

void UCPlayerWeaponComponent::SetupInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed, this, &UCPlayerWeaponComponent::SwitchWeapon);
	PlayerInputComponent->BindAction(TEXT("FireWeapon"), IE_Pressed, this, &UCPlayerWeaponComponent::FireWeapon);
	PlayerInputComponent->BindAction(TEXT("AimWeapon"), IE_Pressed, this, &UCPlayerWeaponComponent::AimWeapon);
	PlayerInputComponent->BindAction(TEXT("AimWeapon"), IE_Released, this, &UCPlayerWeaponComponent::IdleWeapon);
	PlayerInputComponent->BindAction(TEXT("ReloadWeapon"), IE_Pressed, this, &UCPlayerWeaponComponent::ReloadWeapon);
}

void UCPlayerWeaponComponent::BeginPlay()
{
	const FName AnimNotifyStr = TEXT("AnimNotify_PlayerWeaponComponent");

	Super::BeginPlay();

	SetAnimationMode(EAnimationMode::AnimationSingleNode, true);

	auto AnimNotify = NewObject<UCAnimNotify>(this);
	AnimNotify->SetNotifyName(AnimNotifyStr);
	AnimNotify->OnAnimationNotify.AddDynamic(this, &UCPlayerWeaponComponent::OnAnimationEndedNotify);

	AnimationEndedNotifyEvent.NotifyName = AnimNotifyStr;
	AnimationEndedNotifyEvent.Notify = AnimNotify;

	AudioComponent = NewObject<UAudioComponent>(UAudioComponent::StaticClass());
	check(AudioComponent != nullptr);

	AudioComponent->SetupAttachment(this);
	AudioComponent->RegisterComponentWithWorld(GetWorld());

	SetOnlyOwnerSee(true);
	SetVisibility(false);
}

void UCPlayerWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (auto AnimationInstance = GetSingleNodeInstance())
	{
		if (auto AnimSequence = Cast<UAnimSequence>(AnimationInstance->GetAnimationAsset()))
		{
			FAnimNotifyContext Context{};
			AnimSequence->GetAnimNotifiesFromDeltaPositions(
				AnimationInstance->GetCurrentTime(), AnimationInstance->GetCurrentTime(), Context);
			for (auto& ActiveNotify : Context.ActiveNotifies)
			{
				if (const FAnimNotifyEvent* Notify = ActiveNotify.GetNotify())
				{
					if (Notify && !Notify->IsBranchingPoint())
					{
						AnimationInstance->NotifyQueue.AnimNotifies.Add(
							FAnimNotifyEventReference(Notify, AnimSequence));
					}
				}
			}

			AnimationInstance->DispatchQueuedAnimEvents();
		}
	}
}

void UCPlayerWeaponComponent::SwitchToState(EWeaponState NewState)
{
	UAnimSingleNodeInstance* AnimationInstance = GetSingleNodeInstance();
	if (AnimationInstance == nullptr)
	{
		return;
	}

	UAnimSequence* AnimSequence = nullptr;
	USoundBase* Sound = nullptr;

	if (CurrentState == WS_None)
	{
		if (NewState == WS_Raise)
		{
			SetVisibility(true);
			AnimSequence = Cast<UAnimSequence>(RaiseAnimation);
			Sound = RaiseSound;
		}
	}
	else
	{
		if (NewState == WS_Lower)
		{
			AnimSequence = Cast<UAnimSequence>(LowerAnimation);
			Sound = LowerSound;
		}

		if (NewState == WS_Idle)
		{
			AnimSequence = Cast<UAnimSequence>(IdleAnimation);
		}

		if (NewState == WS_Aim)
		{
			AnimSequence = Cast<UAnimSequence>(AimAnimation);
		}

		if (NewState == WS_Fire)
		{
			AnimSequence = Cast<UAnimSequence>(FireAnimation);
			Sound = FireSound;
		}

		if (NewState == WS_Reload)
		{
			AnimSequence = Cast<UAnimSequence>(ReloadAnimation);
			Sound = ReloadSound;
		}

		if (NewState == WS_None)
		{
			SetVisibility(false);
			CurrentState = NewState;
		}
	}

	if (Sound)
	{
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}

	if (AnimSequence != nullptr)
	{
		AnimationEndedNotifyEvent.SetTime(AnimSequence->GetPlayLength());
		AnimSequence->Notifies.Empty();
		AnimSequence->Notifies.Add(AnimationEndedNotifyEvent);
		AnimationInstance->SetAnimationAsset(AnimSequence, false);
		AnimationInstance->PlayAnim();
		CurrentState = NewState;
	}
}

void UCPlayerWeaponComponent::OnAnimationEndedNotify(const FString& Name, UAnimSequenceBase* Animation)
{
	if (CurrentState == WS_Raise ||
		CurrentState == WS_Idle ||
		CurrentState == WS_Reload ||
		CurrentState == WS_Fire)
	{
		SwitchToState(WS_Idle);
	}

	if (CurrentState == WS_Lower)
	{
		SwitchToState(WS_None);
	}

	if (CurrentState == WS_Aim)
	{
		SwitchToState(WS_Aim);
	}
}
