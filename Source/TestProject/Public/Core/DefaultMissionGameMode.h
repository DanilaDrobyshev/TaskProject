/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Systems/Mission/MissionData.h"
#include "DefaultMissionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ADefaultMissionGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    virtual void PostLogin(APlayerController* NewPlayer) override;

private:

    UPROPERTY(EditDefaultsOnly, Category = "Mission")
    TObjectPtr<UMissionData> FirstMission;
};