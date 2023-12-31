// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// UE Includes
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMD2ImporterEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule( ) override;
	virtual void ShutdownModule( ) override;
};
