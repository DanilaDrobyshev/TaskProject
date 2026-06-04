/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATestPlayerController();

	void MissionComplete(); // Но как я и написал на вызове - надо делать в плеерстейте
protected:

	virtual void BeginPlay() override;
};
