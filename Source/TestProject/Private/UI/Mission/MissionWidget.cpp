/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "UI/Mission/MissionWidget.h"
#include "Components/VerticalBox.h"
#include "Systems/Mission/MissionData.h"

void UMissionWidget::StartObjective(const UMissionData* Mission, const int32 ObjectiveIndex)
{
    if (!Mission || !Mission->Objectives.IsValidIndex(ObjectiveIndex)) return;

    const FMissionObjectiveData& Obj = Mission->Objectives[ObjectiveIndex];
    AddObjectiveEntry(Obj.SubMissionTitle, Obj.Description);
}

void UMissionWidget::UpdateObjective(const UMissionData* Mission, const int32 NewIndex)
{
    if (!Mission || !Mission->Objectives.IsValidIndex(NewIndex)) return;


    if (IsValid(CurrentEntry))
        CurrentEntry->MarkCompleted();


    const FMissionObjectiveData& Obj = Mission->Objectives[NewIndex];
    AddObjectiveEntry(Obj.SubMissionTitle, Obj.Description);
}

void UMissionWidget::AddObjectiveEntry(const FText& Title, const FText& Description)
{
    if (!ObjectiveEntryClass || !IsValid(ObjectiveList)) return;

    UObjectiveEntryWidget* Entry = CreateWidget<UObjectiveEntryWidget>(GetOwningPlayer(), ObjectiveEntryClass);
    if (!IsValid(Entry)) return;

    Entry->SetObjectiveText(Title, Description);
    ObjectiveList->AddChild(Entry);
    Entry->PlayAppearAnimation();
    CurrentEntry = Entry;
}
