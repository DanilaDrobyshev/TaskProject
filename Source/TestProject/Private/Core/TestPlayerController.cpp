/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Core/TestPlayerController.h"
#include "Core/TestPlayerCameraManager.h"
#include <Core/TestCharacter.h>

ATestPlayerController::ATestPlayerController()
{
	PlayerCameraManagerClass = ATestPlayerCameraManager::StaticClass();
}

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATestPlayerCameraManager* CamMgr = Cast<ATestPlayerCameraManager>(PlayerCameraManager);
	if (IsValid(CamMgr)) CamMgr->StartIntroSequence();
}

void ATestPlayerController::MissionComplete()
{
	ATestPlayerCameraManager* CamMgr = Cast<ATestPlayerCameraManager>(PlayerCameraManager);

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn) || !IsValid(CamMgr)) return;

	ATestCharacter* ControlledChar = Cast<ATestCharacter>(ControlledPawn);
	if (!IsValid(ControlledChar)) return;

	ControlledChar->PlayMissionCompleteAnimation();
	CamMgr->StartOutroSequence();


}