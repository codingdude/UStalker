// Copyright Epic Games, Inc. All Rights Reserved.

#include "UStalker.h"
#include "Modules/ModuleManager.h"

#include "CPlayerWeaponComponentDetails.h"

class FStalkerGameModuleImpl
	: public IModuleInterface
{
	/**
	 * Returns true if this module hosts gameplay code
	 *
	 * @return True for "gameplay modules", or false for engine code modules, plug-ins, etc.
	 */
	virtual bool IsGameModule() const override
	{
		return true;
	}

	/**
	 * Called right after the module DLL has been loaded and the module object has been created
	 * Load dependent modules here, and they will be guaranteed to be available during ShutdownModule. ie:
	 *
	 * FModuleManager::Get().LoadModuleChecked(TEXT("HTTP"));
	 */
	virtual void StartupModule() override
	{
#if WITH_EDITOR
		FPropertyEditorModule& PropertyModule =
			FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(
			"CPlayerWeaponComponent",
			FOnGetDetailCustomizationInstance::CreateStatic(
				&FPlayerWeaponComponentDetails::MakeInstance)
		);
#endif // WITH_EDITOR
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FStalkerGameModuleImpl, UStalker, "UStalker" );
