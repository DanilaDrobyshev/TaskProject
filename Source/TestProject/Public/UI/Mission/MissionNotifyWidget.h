/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionNotifyWidget.generated.h"
class UMissionData;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMissionNotifyWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintImplementableEvent)
    void ShowMissionStarted(const UMissionData* Mission);

    UFUNCTION(BlueprintImplementableEvent)
    void ShowObjectiveChanged(const UMissionData* Mission, int32 NewIndex);

    UFUNCTION(BlueprintImplementableEvent)
    void ShowMissionCompleted(const UMissionData* Mission);

    UFUNCTION(BlueprintImplementableEvent)
    void ShowMissionFailed(const UMissionData* Mission);
};
