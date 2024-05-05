// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR
#include "CPlayerWeaponComponentDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyEditorModule.h"

TSharedRef<IDetailCustomization> FPlayerWeaponComponentDetails::MakeInstance()
{
	return MakeShareable(new FPlayerWeaponComponentDetails);
}

FPlayerWeaponComponentDetails::FPlayerWeaponComponentDetails()
	: Skeleton(nullptr)
	, bAnimPickerEnabled(false)
{

}

FPlayerWeaponComponentDetails::~FPlayerWeaponComponentDetails()
{
	UnregisterAllMeshPropertyChangedCallers();
}

void FPlayerWeaponComponentDetails::HideCategories(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideCategory(TEXT("Animation"));
	DetailBuilder.HideCategory(TEXT("ComponentTick"));
	DetailBuilder.HideCategory(TEXT("Physics"));
	DetailBuilder.HideCategory(TEXT("Collision"));
	DetailBuilder.HideCategory(TEXT("Clothing"));
	DetailBuilder.HideCategory(TEXT("Lighting"));
	DetailBuilder.HideCategory(TEXT("LeaderPoseComponent"));
	DetailBuilder.HideCategory(TEXT("AnimationRig"));
	DetailBuilder.HideCategory(TEXT("Deformer"));
	DetailBuilder.HideCategory(TEXT("SkinWeights"));
	DetailBuilder.HideCategory(TEXT("Rendering"));
	DetailBuilder.HideCategory(TEXT("HLOD"));
	DetailBuilder.HideCategory(TEXT("PathTracing"));
	DetailBuilder.HideCategory(TEXT("Navigation"));
	DetailBuilder.HideCategory(TEXT("VirtualTexture"));
	DetailBuilder.HideCategory(TEXT("Tags"));
	DetailBuilder.HideCategory(TEXT("ComponentReplication"));
	DetailBuilder.HideCategory(TEXT("Activation"));
	DetailBuilder.HideCategory(TEXT("Variable"));
	DetailBuilder.HideCategory(TEXT("Cooking"));
	DetailBuilder.HideCategory(TEXT("SkeletalMesh"));
	DetailBuilder.HideCategory(TEXT("Optimization"));
	DetailBuilder.HideCategory(TEXT("MaterialParameters"));
	DetailBuilder.HideCategory(TEXT("TextureStreaming"));
	DetailBuilder.HideCategory(TEXT("Mobile"));
	DetailBuilder.HideCategory(TEXT("RayTracing"));
	DetailBuilder.HideCategory(TEXT("AssetUserData"));
	DetailBuilder.HideCategory(TEXT("Replication"));
}

USkeleton* FPlayerWeaponComponentDetails::GetValidSkeletonFromRegisteredMeshes() const
{
	USkeleton* ResultSkeleton = NULL;

	for (auto ObjectIter = SelectedObjects.CreateConstIterator(); ObjectIter; ++ObjectIter)
	{
		USkeletalMeshComponent* const Mesh = Cast<USkeletalMeshComponent>(ObjectIter->Get());
		if (!Mesh || !Mesh->GetSkeletalMeshAsset())
		{
			continue;
		}

		// If we've not come across a valid skeleton yet, store this one.
		if (!ResultSkeleton)
		{
			ResultSkeleton = Mesh->GetSkeletalMeshAsset()->GetSkeleton();
			continue;
		}

		// We've encountered a valid skeleton before.
		// If this skeleton is not the same one, that means there are multiple
		// skeletons selected, so we don't want to take any action.
		if (Mesh->GetSkeletalMeshAsset()->GetSkeleton() != ResultSkeleton)
		{
			return NULL;
		}
	}

	return ResultSkeleton;
}

void FPlayerWeaponComponentDetails::SkeletalMeshPropertyChanged()
{
	UpdateSkeletonNameAndPickerVisibility();
}

void FPlayerWeaponComponentDetails::UpdateSkeletonNameAndPickerVisibility()
{
	// Update the selected skeleton name and the picker visibility
	Skeleton = GetValidSkeletonFromRegisteredMeshes();

	if (Skeleton)
	{
		bAnimPickerEnabled = true;
	}
	else
	{
		bAnimPickerEnabled = false;
	}
}

void FPlayerWeaponComponentDetails::RegisterSkeletalMeshPropertyChanged(TWeakObjectPtr<USkeletalMeshComponent> Mesh)
{
	if (Mesh.IsValid() && OnSkeletalMeshPropertyChanged.IsBound())
	{
		OnSkeletalMeshPropertyChangedDelegateHandles.Add(Mesh.Get(), Mesh->RegisterOnSkeletalMeshPropertyChanged(OnSkeletalMeshPropertyChanged));
	}
}

void FPlayerWeaponComponentDetails::PerformInitialRegistrationOfSkeletalMeshes(IDetailLayoutBuilder& DetailBuilder)
{
	OnSkeletalMeshPropertyChanged = USkeletalMeshComponent::FOnSkeletalMeshPropertyChanged::CreateSP(this, &FPlayerWeaponComponentDetails::SkeletalMeshPropertyChanged);

	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	check(SelectedObjects.Num() > 0);

	for (auto ObjectIter = SelectedObjects.CreateIterator(); ObjectIter; ++ObjectIter)
	{
		if (USkeletalMeshComponent* Mesh = Cast<USkeletalMeshComponent>(ObjectIter->Get()))
		{
			RegisterSkeletalMeshPropertyChanged(Mesh);
		}
	}
}

void FPlayerWeaponComponentDetails::UnregisterAllMeshPropertyChangedCallers()
{
	for (auto MeshIter = SelectedObjects.CreateIterator(); MeshIter; ++MeshIter)
	{
		if (USkeletalMeshComponent* Mesh = Cast<USkeletalMeshComponent>(MeshIter->Get()))
		{
			Mesh->UnregisterOnSkeletalMeshPropertyChanged(OnSkeletalMeshPropertyChangedDelegateHandles.FindRef(Mesh));
			OnSkeletalMeshPropertyChangedDelegateHandles.Remove(Mesh);
		}
	}
}

bool FPlayerWeaponComponentDetails::OnShouldFilterAnimAsset(const FAssetData& AssetData)
{
	// Check the compatible skeletons.
	if (Skeleton && Skeleton->IsCompatibleForEditor(AssetData))
	{
		return false;
	}

	return true;
}

bool FPlayerWeaponComponentDetails::AnimPickerIsEnabled() const
{
	return bAnimPickerEnabled;
}

void FPlayerWeaponComponentDetails::UpdateAnimationProperty(IDetailLayoutBuilder& DetailBuilder, const FName& PropertyName)
{
	IDetailCategoryBuilder& AnimationsCategory =
		DetailBuilder.EditCategory(TEXT("Animations"), FText::GetEmpty(), ECategoryPriority::Important);

	TSharedPtr<IPropertyHandle> AnimationHandle = DetailBuilder.GetProperty(PropertyName);

	// Hide the property, as we're about to add it differently
	DetailBuilder.HideProperty(AnimationHandle);

	// Add it differently
	TSharedPtr<SWidget> NameWidget = AnimationHandle->CreatePropertyNameWidget();

	TSharedRef<SWidget> PropWidget = SNew(SObjectPropertyEntryBox)
		.ThumbnailPool(DetailBuilder.GetThumbnailPool())
		.PropertyHandle(AnimationHandle)
		.AllowedClass(UAnimationAsset::StaticClass())
		.AllowClear(true)
		.OnShouldFilterAsset(FOnShouldFilterAsset::CreateSP(this, &FPlayerWeaponComponentDetails::OnShouldFilterAnimAsset));

	TAttribute<bool> AnimPickerEnabledAttr(this, &FPlayerWeaponComponentDetails::AnimPickerIsEnabled);

	AnimationsCategory.AddProperty(AnimationHandle)
		.CustomWidget()
		.IsEnabled(AnimPickerEnabledAttr)
		.NameContent()
		[
			NameWidget.ToSharedRef()
		]
		.ValueContent()
		.MinDesiredWidth(600)
		.MaxDesiredWidth(600)
		[
			PropWidget
		]
		.PropertyHandleList({ AnimationHandle });
}

void FPlayerWeaponComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	HideCategories(DetailBuilder);
	PerformInitialRegistrationOfSkeletalMeshes(DetailBuilder);
	UpdateAnimationProperty(DetailBuilder, TEXT("RaiseAnimation"));
	UpdateAnimationProperty(DetailBuilder, TEXT("LowerAnimation"));
	UpdateAnimationProperty(DetailBuilder, TEXT("IdleAnimation"));
	UpdateAnimationProperty(DetailBuilder, TEXT("AimAnimation"));
	UpdateAnimationProperty(DetailBuilder, TEXT("FireAnimation"));
	UpdateAnimationProperty(DetailBuilder, TEXT("ReloadAnimation"));
}
#endif // WITH_EDITOR
