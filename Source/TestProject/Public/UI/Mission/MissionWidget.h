/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveEntryWidget.h"
#include "MissionWidget.generated.h"

class UMissionData;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    // Добавить новую задачу в список
    UFUNCTION(BlueprintImplementableEvent)
    void ShowMissionStarted(const UMissionData* Mission);

    // Скрыть задачу
    UFUNCTION(BlueprintImplementableEvent)
    void HideMission(const UMissionData* Mission);

    // Добавить подзадачу в список
    void AddObjectiveEntry(const FText& Title, const FText& Description);

    // Пометить текущую как выполненную и добавить следующую
    void UpdateObjective(const UMissionData* Mission, int32 NewIndex);

    // Первая задача
    void StartObjective(const UMissionData* Mission, int32 ObjectiveIndex);

protected:

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UVerticalBox> ObjectiveList;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UObjectiveEntryWidget> ObjectiveEntryClass;

private:

    // Текущий активный entry — чтобы пометить его выполненным
    UPROPERTY()
    TObjectPtr<UObjectiveEntryWidget> CurrentEntry;
};
