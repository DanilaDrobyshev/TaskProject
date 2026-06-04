/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Systems/Mission/MissionData.h"
#include "MissionHUD.generated.h"

class UMissionWidget;
class UMissionNotifyWidget;
class UMissionSubsystem;

UCLASS()
class TESTPROJECT_API AMissionHUD : public AHUD
{
    GENERATED_BODY()

public:

    virtual void BeginPlay() override;

protected:

    // Виджет для показа текущей миссии
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMissionWidget> MissionWidgetClass;

    // Виджет для показа уведомлений о выполнении/проваливании миссии
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMissionNotifyWidget> NotifyWidgetClass;

private:

    UPROPERTY()
    TObjectPtr<UMissionWidget> MissionWidget;

    UPROPERTY()
    TObjectPtr<UMissionNotifyWidget> NotifyWidget;

    void OnMissionStarted(UMissionData* Mission);
    void OnMissionCompleted(UMissionData* Mission);
    void OnMissionFailed(UMissionData* Mission);
    void OnObjectiveChanged(UMissionData* Mission, int32 NewIndex);
};