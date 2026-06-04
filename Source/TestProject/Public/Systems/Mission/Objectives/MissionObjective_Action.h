/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Systems/Mission/MissionObjectiveBase.h"
#include "MissionObjective_Action.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMissionObjective_Action : public UMissionObjectiveBase
{
	GENERATED_BODY()
public:
    virtual void Activate(const FMissionObjectiveData& InData, AActor* InInstigator) override;
    virtual void Deactivate() override;

private:
    TWeakObjectPtr<AActor> TargetActor;

    void OnActionPerformed(AActor* Interactor);
};
