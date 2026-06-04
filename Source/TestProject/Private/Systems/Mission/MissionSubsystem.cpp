/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Systems/Mission/MissionSubsystem.h"
#include <Systems/Mission/MissionData.h>
#include <Core/TestPlayerController.h>


void UMissionSubsystem::StartMission(UMissionData* MissionData, AActor* Instigator)
{
    if (!MissionData || !Instigator) return;

    const FName ID = MissionData->MissionID;

    if (ActiveMissions.Contains(ID) || CompletedMissionIDs.Contains(ID)) return;
    if (MissionData->Objectives.IsEmpty()) return;

    FActiveMission& Mission = ActiveMissions.Add(ID);
    Mission.Data = MissionData;
    Mission.State = EMissionState::Active;
    Mission.CurrentObjectiveIndex = 0;

    Mission.Objectives.SetNum(MissionData->Objectives.Num());

    ActivateObjective(Mission, 0, Instigator);
    OnMissionStarted.Broadcast(MissionData);
}

void UMissionSubsystem::CompleteMission(const FName MissionID, AActor* Instigator)
{
    FActiveMission* Mission = ActiveMissions.Find(MissionID);
    if (!Mission) return;

    for (const TObjectPtr<UMissionObjectiveBase>& Obj : Mission->Objectives)
        if (IsValid(Obj) && Obj->GetState() == EObjectiveState::Active)
            Obj->Deactivate();

    Mission->State = EMissionState::Completed;
    CompletedMissionIDs.Add(MissionID);
    OnMissionCompleted.Broadcast(Mission->Data);
    ActiveMissions.Remove(MissionID);

    // Обрабатывать повышение уровня и соответственно выполнения задания надо через playerstate, но щас без него иначе проект разрастется
    {
        // Будем просто запускать анимацию победы и эффект камеры
        ATestPlayerController* PC = Cast<ATestPlayerController>(Instigator);
        if (!IsValid(PC)) return;

        PC->MissionComplete();
    }
}

void UMissionSubsystem::FailMission(const FName MissionID, AActor* Instigator)
{
    FActiveMission* Mission = ActiveMissions.Find(MissionID);
    if (!Mission) return;

    for (const TObjectPtr<UMissionObjectiveBase>& Obj : Mission->Objectives)
        if (IsValid(Obj) && Obj->GetState() == EObjectiveState::Active)
            Obj->Deactivate();

    Mission->State = EMissionState::Failed;
    OnMissionFailed.Broadcast(Mission->Data);
    ActiveMissions.Remove(MissionID);
}

FORCEINLINE const FMissionObjectiveData* UMissionSubsystem::GetCurrentObjectiveData(const FName MissionID) const
{
    const FActiveMission* Mission = ActiveMissions.Find(MissionID);
    if (!Mission || !IsValid(Mission->Data)) return nullptr;

    const int32 Idx = Mission->CurrentObjectiveIndex;
    return Mission->Data->Objectives.IsValidIndex(Idx) ? &Mission->Data->Objectives[Idx] : nullptr;
}

void UMissionSubsystem::ActivateObjective(FActiveMission& Mission, const int32 Index, AActor* Instigator)
{
    const TSubclassOf<UMissionObjectiveBase>& ObjClass = Mission.Data->Objectives[Index].ObjectiveClass;
    if (!IsValid(ObjClass)) return;

    UMissionObjectiveBase* Obj = NewObject<UMissionObjectiveBase>(this, ObjClass);
    if (!IsValid(Obj)) return;

    Mission.Objectives[Index] = Obj;

    const FName MissionID = Mission.Data->MissionID;

    Obj->OnCompleted.AddLambda([this, MissionID, Instigator](UMissionObjectiveBase* /*Completed*/)
        {
            OnObjectiveCompleted(MissionID, Instigator);
        });

    Obj->OnFailed.AddLambda([this, MissionID, Instigator](UMissionObjectiveBase* /*Failed*/)
        {
            OnObjectiveFailed(MissionID, Instigator);
        });

    Obj->Activate(Mission.Data->Objectives[Index], Instigator);
}

void UMissionSubsystem::OnObjectiveCompleted(const FName MissionID, AActor* Instigator)
{
    FActiveMission* Mission = ActiveMissions.Find(MissionID);
    if (!Mission) return;

    const int32 NextIndex = Mission->CurrentObjectiveIndex + 1;

    if (!Mission->Data->Objectives.IsValidIndex(NextIndex))
    {
        CompleteMission(MissionID,Instigator);
        return;
    }

    Mission->CurrentObjectiveIndex = NextIndex;
    OnObjectiveChanged.Broadcast(Mission->Data, NextIndex);
    ActivateObjective(*Mission, NextIndex, Instigator);
}

void UMissionSubsystem::OnObjectiveFailed(const FName MissionID, AActor* Instigator)
{
    FailMission(MissionID, Instigator);
}