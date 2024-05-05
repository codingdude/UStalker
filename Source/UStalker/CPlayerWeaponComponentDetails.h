// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
#include "IDetailCustomization.h"

class FPlayerWeaponComponentDetails
	: public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	FPlayerWeaponComponentDetails();
	~FPlayerWeaponComponentDetails();

	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	void HideCategories(IDetailLayoutBuilder& DetailBuilder);

	USkeleton* GetValidSkeletonFromRegisteredMeshes() const;

	void SkeletalMeshPropertyChanged();

	void UpdateSkeletonNameAndPickerVisibility();

	void RegisterSkeletalMeshPropertyChanged(TWeakObjectPtr<USkeletalMeshComponent> Mesh);

	void PerformInitialRegistrationOfSkeletalMeshes(IDetailLayoutBuilder& DetailBuilder);

	void UnregisterAllMeshPropertyChangedCallers();

	bool OnShouldFilterAnimAsset(const FAssetData& AssetData);

	bool AnimPickerIsEnabled() const;

	void UpdateAnimationProperty(IDetailLayoutBuilder& DetailBuilder, const FName& PropertyName);

private:
	/** Delegate called when a skeletal mesh property is changed on a selected object */
	USkeletalMeshComponent::FOnSkeletalMeshPropertyChanged OnSkeletalMeshPropertyChanged;

	/** Cached selected objects to use when the skeletal mesh property changes */
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;

	/** The skeleton that we grab the name from for filtering. */
	USkeleton* Skeleton;

	/** Current enabled state of the animation asset picker in the details panel */
	bool bAnimPickerEnabled;

	/** Per-mesh handles to registered OnSkeletalMeshPropertyChanged delegates */
	TMap<USkeletalMeshComponent*, FDelegateHandle> OnSkeletalMeshPropertyChangedDelegateHandles;
};
#endif // WITH_EDITOR
