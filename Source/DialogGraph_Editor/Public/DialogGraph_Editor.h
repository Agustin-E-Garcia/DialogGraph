#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

class DialogStyleSet;
struct FGraphNodeClassHelper;

class FDialogGraph_EditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:

    //~ Begin IModuleInterface interface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //~ End IModuleInterface interface

    virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override;
    virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override;

    static const FName DialogAssetEditorAppIdentifier;

private:
    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
    TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};
