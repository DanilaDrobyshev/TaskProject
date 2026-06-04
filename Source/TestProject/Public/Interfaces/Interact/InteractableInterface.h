/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECT_API IInteractableInterface
{
	GENERATED_BODY()

public:

    // Вызывается персонажем при нажатии кнопки взаимодействия
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnInteract(AActor* Interactor);
    virtual void OnInteract_Implementation(AActor* Interactor) = 0;

    // Для проверки или UI подсказки  
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract(AActor* Interactor) const;
    virtual bool CanInteract_Implementation(AActor* Interactor) const { return true; }

    // Objective_Action подписывается на этот делегат в Activate()
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*)
    virtual FOnInteracted& GetOnInteractedDelegate() = 0;
};
