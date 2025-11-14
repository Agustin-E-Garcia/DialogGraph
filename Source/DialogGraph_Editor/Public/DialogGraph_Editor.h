// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include <Styling/SlateStyle.h>

class DialogStyleSet;

class FDialogGraph_EditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
    TSharedPtr<DialogStyleSet> _styleSet = nullptr;
};
