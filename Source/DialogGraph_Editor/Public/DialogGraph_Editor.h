#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

class DialogStyleSet;
struct FGraphNodeClassHelper;

class FDialogGraph_EditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
    virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }

    TSharedPtr<FGraphNodeClassHelper> GetClassCache() const { return ClassCache; }

    static const FName DialogAssetEditorAppIdentifier;

private:
    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
    TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;

    TSharedPtr<FGraphNodeClassHelper> ClassCache;

    TSharedPtr<DialogStyleSet> _styleSet = nullptr;
};
