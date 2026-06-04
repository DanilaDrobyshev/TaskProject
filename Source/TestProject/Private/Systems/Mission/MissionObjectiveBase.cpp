/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Systems/Mission/MissionObjectiveBase.h"

void UMissionObjectiveBase::MarkCompleted()
{
    if (State == EObjectiveState::Completed) return;

    Deactivate();
    State = EObjectiveState::Completed;
    Progress = 1.f;
    OnCompleted.Broadcast(this);
}

void UMissionObjectiveBase::MarkFailed()
{
    if (State == EObjectiveState::Failed) return;

    Deactivate();
    State = EObjectiveState::Failed;
    OnFailed.Broadcast(this);
}