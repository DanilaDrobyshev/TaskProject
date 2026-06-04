/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionObjectiveBase.h"
#include "MissionSubsystem.generated.h"

class UMissionData;

USTRUCT()
struct FActiveMission
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UMissionData> Data = nullptr;

    UPROPERTY()
    TArray<TObjectPtr<UMissionObjectiveBase>> Objectives;

    EMissionState State = EMissionState::Inactive;
    int32         CurrentObjectiveIndex = 0;
};

UCLASS()
class TESTPROJECT_API UMissionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    void StartMission(UMissionData* MissionData, AActor* Instigator);
    void CompleteMission(const FName MissionID, AActor* Instigator);
    void FailMission(const FName MissionID, AActor* Instigator);

    FORCEINLINE bool IsMissionActive(const FName MissionID)    const { return ActiveMissions.Contains(MissionID); }
    FORCEINLINE bool IsMissionCompleted(const FName MissionID) const { return CompletedMissionIDs.Contains(MissionID); }

    FORCEINLINE const FActiveMission* GetActiveMission(const FName MissionID)        const { return ActiveMissions.Find(MissionID); }
    FORCEINLINE const FMissionObjectiveData* GetCurrentObjectiveData(const FName MissionID) const;

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnMissionStarted, UMissionData*)
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnMissionCompleted, UMissionData*)
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnMissionFailed, UMissionData*)
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectiveChanged, UMissionData*, int32)

    FOnMissionStarted   OnMissionStarted;
    FOnMissionCompleted OnMissionCompleted;
    FOnMissionFailed    OnMissionFailed;
    FOnObjectiveChanged OnObjectiveChanged;

private:

    TMap<FName, FActiveMission> ActiveMissions;
    TSet<FName>                 CompletedMissionIDs;

    void ActivateObjective(FActiveMission& Mission, int32 Index, AActor* Instigator);
    void OnObjectiveCompleted(const FName MissionID, AActor* Instigator);
    void OnObjectiveFailed(const FName MissionID, AActor* Instigator);
};