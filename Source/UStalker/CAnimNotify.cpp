// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify.h"

UCAnimNotify::UCAnimNotify(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UCAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnAnimationNotify.Broadcast(NotifyName.ToString(), Animation);
}

void UCAnimNotify::SetNotifyName(const FName& InNotifyName)
{
	NotifyName = InNotifyName;
}

FString UCAnimNotify::GetNotifyName_Implementation() const
{
	return NotifyName.ToString();
}
