#include <DialogAssetAction.h>
#include <DialogAsset.h>
#include <DialogAssetEditor.h>

FDialogAssetAction::FDialogAssetAction(EAssetTypeCategories::Type category)
{
    _assetCategory = category;
}

FText FDialogAssetAction::GetName() const { return FText::FromString(TEXT("Dialog Asset")); }

FColor FDialogAssetAction::GetTypeColor() const { return FColor::Cyan; }

UClass* FDialogAssetAction::GetSupportedClass() const { return UDialogAsset::StaticClass(); }

void FDialogAssetAction::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor) 
{
    EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    
    for(UObject* object : inObjects)
    {
        UDialogAsset* dialogAsset = Cast<UDialogAsset>(object);
        if(dialogAsset != nullptr)
        {
            TSharedRef<FDialogAssetEditor> editor(new FDialogAssetEditor());
            editor->InitEditor(mode, editWithinLevelEditor, dialogAsset);
        }
    }
}

uint32 FDialogAssetAction::GetCategories() { return _assetCategory; }
