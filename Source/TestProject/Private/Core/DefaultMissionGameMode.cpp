// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DefaultMissionGameMode.h"
#include "Systems/Mission/MissionSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ADefaultMissionGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!FirstMission) return;

    // Небольшая задержка чтобы HUD успел подписаться
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
        {
            UMissionSubsystem* MS = GetGameInstance()->GetSubsystem<UMissionSubsystem>();
            if (IsValid(MS)) MS->StartMission(FirstMission, NewPlayer);

        }, 0.2f, false);
}