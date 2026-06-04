/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Systems/Mission/Objectives/MissionObjective_Reach.h"
#include <Components/WidgetComponent.h>
#include "EngineUtils.h"
#include "TimerManager.h"


void UMissionObjective_Reach::Activate(const FMissionObjectiveData& InData, AActor* InInstigator)
{
    if (!InInstigator) { MarkFailed(); return; }

    Data = InData;
    Instigator = InInstigator;

    UWorld* World = InInstigator->GetWorld();
    if (!IsValid(World)) { MarkFailed(); return; }

    TargetActor = Data.TargetActor.Get();

    if (!TargetActor.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Objective_Reach: TargetActor is not set or not loaded"));
        MarkFailed();
        return;
    }

    World->GetTimerManager().SetTimer(TimerHandle, this, &UMissionObjective_Reach::CheckDistance, 0.2f, true);

    SpawnMarkerAsync();
}

void UMissionObjective_Reach::Deactivate()
{
    if (UWorld* World = GetWorld())
        World->GetTimerManager().ClearTimer(TimerHandle);

    TargetActor.Reset();
    DestroyMarker();
}

void UMissionObjective_Reach::CheckDistance()
{
    if (!Instigator.IsValid() || !TargetActor.IsValid()) { MarkFailed(); return; }

    AController* PC = Cast<AController>(Instigator);
    if (!IsValid(PC)) return;

    APawn* PP = PC->GetPawn();
    if (!IsValid(PP)) return;

    const FVector InstigatorLoc = PP->GetActorLocation();
    const FVector TargetLoc = TargetActor->GetActorLocation();
    const float   Dist = FVector::Dist(InstigatorLoc, TargetLoc);

    Progress = FMath::Clamp(1.f - Dist / (Data.AcceptanceRadius * 3.f), 0.f, 1.f);

    if (Dist <= Data.AcceptanceRadius) MarkCompleted();
}

void UMissionObjective_Reach::SpawnMarkerAsync()
{
    if (Data.MarkerWidgetClass.IsNull()) return;

    TWeakObjectPtr<UMissionObjective_Reach> WeakThis(this);
    TSoftClassPtr<UUserWidget> SoftClass = Data.MarkerWidgetClass;

    MarkerLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
        SoftClass.ToSoftObjectPath(),
        [WeakThis, SoftClass]()
        {
            if (WeakThis.IsValid())
                WeakThis->OnMarkerLoaded(SoftClass);
        }
    );
}

void UMissionObjective_Reach::OnMarkerLoaded(TSoftClassPtr<UUserWidget> SoftClass)
{
    if (!TargetActor.IsValid()) return;

    UClass* WidgetClass = SoftClass.Get();
    if (!WidgetClass) return;

    UWidgetComponent* Comp = NewObject<UWidgetComponent>(TargetActor.Get());
    if (!IsValid(Comp)) return;

    Comp->SetWidgetClass(WidgetClass);
    Comp->SetWidgetSpace(EWidgetSpace::Screen); // через стены видно
    Comp->RegisterComponent();
    Comp->InitWidget();
    Comp->AttachToComponent(
        TargetActor->GetRootComponent(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    MarkerComponent = Comp;
}

void UMissionObjective_Reach::DestroyMarker()
{
    if (MarkerLoadHandle.IsValid())
    {
        MarkerLoadHandle->CancelHandle();
        MarkerLoadHandle.Reset();
    }

    if (MarkerComponent.IsValid())
    {
        MarkerComponent->DestroyComponent();
        MarkerComponent.Reset();
    }
}