// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimNotifyDelegate, const FString&, NotifyName, UAnimSequenceBase*, Animation);

UCLASS()
class USTALKER_API UCAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UCAnimNotify(const FObjectInitializer& ObjectInitializer);

	void SetNotifyName(const FName& InNotifyName);

	virtual FString GetNotifyName_Implementation() const override;

	FAnimNotifyDelegate OnAnimationNotify;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	FName NotifyName;
};
