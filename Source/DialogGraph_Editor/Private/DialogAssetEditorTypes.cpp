#include "DialogAssetEditorTypes.h"

#include "UObject/NameTypes.h"

const FName UDialogAssetEditorTypes::PinCategory_SingleNode("SingleNode");
const FName UDialogAssetEditorTypes::PinCategory_MultipleNodes("MultipleNodes");
const FName UDialogAssetEditorTypes::PinSubCategory_ChoiceNode("ChoiceNode");

UDialogAssetEditorTypes::UDialogAssetEditorTypes(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
