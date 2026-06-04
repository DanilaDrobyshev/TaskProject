/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MissionObjectiveBase.h"
#include "MissionData.generated.h"


// —оздаЄтс€ в редакторе: ѕ ћ -> Miscellaneous -> Data Asset -> UMissionData
UCLASS(BlueprintType)
class TESTPROJECT_API UMissionData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission")
    FName MissionID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission")
    FText Title;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission")
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission")
    TObjectPtr<UTexture2D> Icon;

    // —писок задач Ч выполн€ютс€ последовательно
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission")
    TArray<FMissionObjectiveData> Objectives;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("Mission", MissionID);
    }
};