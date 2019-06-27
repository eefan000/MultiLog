// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MultiLogType.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UMultiLogSubsystem;

class FMultiLogModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
