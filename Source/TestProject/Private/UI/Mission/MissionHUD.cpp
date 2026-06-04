/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "UI/Mission/MissionHUD.h"
#include "UI/Mission/MissionWidget.h"
#include "UI/Mission/MissionNotifyWidget.h"
#include "Systems/Mission/MissionSubsystem.h"
#include "Systems/Mission/MissionData.h"

void AMissionHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MissionWidgetClass)
    {
        MissionWidget = CreateWidget<UMissionWidget>(GetWorld(), MissionWidgetClass);
        if (IsValid(MissionWidget))
        {
            MissionWidget->AddToViewport();
            MissionWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (NotifyWidgetClass)
    {
        NotifyWidget = CreateWidget<UMissionNotifyWidget>(GetWorld(), NotifyWidgetClass);
        if (IsValid(NotifyWidget))
        {
            NotifyWidget->AddToViewport();
            NotifyWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    UMissionSubsystem* MS = GetGameInstance()->GetSubsystem<UMissionSubsystem>();
    if (!MS) return;

    MS->OnMissionStarted.AddUObject(this, &AMissionHUD::OnMissionStarted);
    MS->OnMissionCompleted.AddUObject(this, &AMissionHUD::OnMissionCompleted);
    MS->OnMissionFailed.AddUObject(this, &AMissionHUD::OnMissionFailed);
    MS->OnObjectiveChanged.AddUObject(this, &AMissionHUD::OnObjectiveChanged);
}

void AMissionHUD::OnMissionStarted(UMissionData* Mission)
{
    if (!Mission || !IsValid(MissionWidget) || !IsValid(NotifyWidget)) return;

    MissionWidget->ShowMissionStarted(Mission);
    MissionWidget->StartObjective(Mission, 0);
    NotifyWidget->ShowMissionStarted(Mission);
}

void AMissionHUD::OnObjectiveChanged(UMissionData* Mission, int32 NewIndex)
{
    if (!Mission || !IsValid(MissionWidget) || !IsValid(NotifyWidget)) return;
    if (!Mission->Objectives.IsValidIndex(NewIndex)) return;

    MissionWidget->UpdateObjective(Mission, NewIndex);
    NotifyWidget->ShowObjectiveChanged(Mission, NewIndex);
}

void AMissionHUD::OnMissionCompleted(UMissionData* Mission)
{
    if (!Mission || !IsValid(NotifyWidget) || !IsValid(MissionWidget)) return;

    NotifyWidget->ShowMissionCompleted(Mission);
    MissionWidget->HideMission(Mission);
}

void AMissionHUD::OnMissionFailed(UMissionData* Mission)
{
    if (!Mission || !IsValid(NotifyWidget) || !IsValid(MissionWidget)) return;

    NotifyWidget->ShowMissionFailed(Mission);
    MissionWidget->HideMission(Mission);
}