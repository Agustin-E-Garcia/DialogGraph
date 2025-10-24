#include <EditorGraph/DialogGraphNode.h>
#include <Framework/Commands/UIAction.h>
#include <ToolMenu.h>

void UDialogChoiceGraphNode::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
    FToolMenuSection& section = menu->AddSection(TEXT("DialogChoiceSection"), FText::FromString(TEXT("Dialog Choice Actions")));
}
