// Copyright Epic Games, Inc. All Rights Reserved.
#include "DialogGraph_Editor.h"
#include "AssetCreation/DialogAssetAction.h"
#include "DialogAssetEditorTypes.h"
#include "DialogAssetGraphNode.h"
#include "EdGraphUtilities.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "SGraphPin.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "AIGraphTypes.h"
#include "SGraphNode_DialogGraph.h"
#include "SGraphPin_DialogGraph.h"

#define LOCTEXT_NAMESPACE "FDialogGraph_EditorModule"

const FName FDialogGraph_EditorModule::DialogAssetEditorAppIdentifier(TEXT("DialogAssetEditorApp"));

class FGraphPanelNodeFactory_DialogAsset : public FGraphPanelNodeFactory
{
    virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
    {
        if(UDialogAssetGraphNode* DialogGraphNode = Cast<UDialogAssetGraphNode>(Node))
        {
            return SNew(SGraphNode_DialogGraph, DialogGraphNode);
        }

        return nullptr;
    }
};

class FGraphPanelPinFactory_DialogAsset : public FGraphPanelPinFactory
{
    virtual TSharedPtr<class SGraphPin> CreatePin(UEdGraphPin* Pin) const override
    {
        if(Pin->PinType.PinCategory == UDialogAssetEditorTypes::PinCategory_SingleNode ||
           Pin->PinType.PinCategory == UDialogAssetEditorTypes::PinCategory_MultipleNodes)
        {
            return SNew(SGraphPin_DialogGraph, Pin);
        }

        return nullptr;
    }
};

TSharedPtr<FGraphPanelNodeFactory_DialogAsset> GraphPanelNodeFactory_DialogAsset;
TSharedPtr<FGraphPanelPinFactory_DialogAsset> GraphPanelPinFactory_DialogAsset;

void FDialogGraph_EditorModule::StartupModule()
{
    MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
    ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

    GraphPanelNodeFactory_DialogAsset = MakeShareable(new FGraphPanelNodeFactory_DialogAsset());
    FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_DialogAsset);
    GraphPanelPinFactory_DialogAsset = MakeShareable(new FGraphPanelPinFactory_DialogAsset());
    FEdGraphUtilities::RegisterVisualPinFactory(GraphPanelPinFactory_DialogAsset);

    IAssetTools& assetToolsModule = IAssetTools::Get();
    EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("Custom Assets")), FText::FromString("Dialog Asset"));
    TSharedPtr<FDialogAssetAction> dialogAssetAction = MakeShareable(new FDialogAssetAction(assetType));
    assetToolsModule.RegisterAssetTypeActions(dialogAssetAction.ToSharedRef());

    if(!ClassCache.IsValid())
    {
        ClassCache = MakeShareable(new FGraphNodeClassHelper(UDialogAssetGraphNode::StaticClass()));
    }
}

void FDialogGraph_EditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogGraph_EditorModule, DialogGraph_Editor)
