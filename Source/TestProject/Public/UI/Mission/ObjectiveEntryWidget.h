// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UObjectiveEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintImplementableEvent)
    void PlayAppearAnimation();

    UFUNCTION(BlueprintImplementableEvent)
    void SetObjectiveText(const FText& Title, const FText& Description);

    UFUNCTION(BlueprintImplementableEvent)
    void MarkCompleted();
};
