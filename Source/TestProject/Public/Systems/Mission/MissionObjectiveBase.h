/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MissionObjectiveBase.generated.h"


UENUM(BlueprintType)
enum class EObjectiveState : uint8
{
    Inactive,
    Active,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EMissionState : uint8
{
    Inactive,
    Active,
    Completed,
    Failed
};

USTRUCT(BlueprintType)
struct FMissionObjectiveData
{
    GENERATED_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText SubMissionTitle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UMissionObjectiveBase> ObjectiveClass;

    // Для статичных акторов, например дойти до точки
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<AActor> TargetActor;

    // Для динамических (враги, предметы)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> TargetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AcceptanceRadius = 300.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 RequiredCount = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName CustomEventID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftClassPtr<UUserWidget> MarkerWidgetClass;
};

// -------------------------------------------------------

UCLASS(Abstract)
class TESTPROJECT_API UMissionObjectiveBase : public UObject
{
    GENERATED_BODY()

public:

    virtual void Activate(const FMissionObjectiveData& InData, AActor* InInstigator) PURE_VIRTUAL(UMissionObjectiveBase::Activate, );
    virtual void Deactivate() PURE_VIRTUAL(UMissionObjectiveBase::Deactivate, );

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UMissionObjectiveBase*)
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveFailed, UMissionObjectiveBase*)

    FOnObjectiveCompleted OnCompleted;
    FOnObjectiveFailed    OnFailed;

    FORCEINLINE EObjectiveState              GetState()    const { return State; }
    FORCEINLINE float                        GetProgress() const { return Progress; }
    FORCEINLINE bool                         IsCompleted() const { return State == EObjectiveState::Completed; }
    FORCEINLINE const FMissionObjectiveData& GetData()     const { return Data; }

protected:

    void MarkCompleted();
    void MarkFailed();

    FMissionObjectiveData  Data;
    TWeakObjectPtr<AActor> Instigator;
    float                  Progress = 0.f;

private:

    EObjectiveState State = EObjectiveState::Inactive;
};