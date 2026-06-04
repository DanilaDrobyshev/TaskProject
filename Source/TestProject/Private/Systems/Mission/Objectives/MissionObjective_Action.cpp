/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Systems/Mission/Objectives/MissionObjective_Action.h"
#include "Interfaces/Interact/InteractableInterface.h"
#include "EngineUtils.h"

void UMissionObjective_Action::Activate(const FMissionObjectiveData& InData, AActor* InInstigator)
{
    if (!IsValid(InInstigator)) { MarkFailed(); return; }

    Data = InData;
    Instigator = InInstigator;

    TargetActor = Data.TargetActor.Get();

    if (!TargetActor.IsValid() || !TargetActor->Implements<UInteractableInterface>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Objective_Action: TargetActor is not set or does not implement IInteractableInterface"));
        MarkFailed();
        return;
    }

    IInteractableInterface* Interactable = Cast<IInteractableInterface>(TargetActor.Get());
    if (Interactable)
        Interactable->GetOnInteractedDelegate().AddUObject(this, &UMissionObjective_Action::OnActionPerformed);
}

void UMissionObjective_Action::Deactivate()
{
    if (!TargetActor.IsValid()) return;

    IInteractableInterface* Interactable = Cast<IInteractableInterface>(TargetActor.Get());
    if (Interactable)
        Interactable->GetOnInteractedDelegate().RemoveAll(this);

    TargetActor.Reset();
}

void UMissionObjective_Action::OnActionPerformed(AActor* Interactor)
{
    MarkCompleted();
}