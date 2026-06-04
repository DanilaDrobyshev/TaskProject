/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interact/InteractableInterface.h"
#include "Components/WidgetComponent.h"
#include "DoorWithCodelock.generated.h"

class UCodelockWidget;

UCLASS()
class TESTPROJECT_API ADoorWithCodelock : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:

    ADoorWithCodelock();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> FrameMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;


    virtual void OnInteract_Implementation(AActor* Interactor) override;
    virtual bool CanInteract_Implementation(AActor* Interactor) const override;
    virtual IInteractableInterface::FOnInteracted& GetOnInteractedDelegate() override { return OnInteractedDelegate; }


    void OnCorrectPassword();
    void OnWrongPassword();
    void ClosePasswordWidget();

    const FString& GetCorrectPassword() const { return CorrectPassword; }

protected:
    virtual void BeginPlay() override;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<USceneComponent> HingePivot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UWidgetComponent> KeypadWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    FString CorrectPassword = TEXT("1234");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float OpenAngle = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float OpenSpeed = 80.f;

    FTimerHandle OpenTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    TSubclassOf<UCodelockWidget> PasswordWidgetClass;

private:
    bool bIsOpen = false;
    float CurrentAngle = 0.f;

    TWeakObjectPtr<AActor> CurrentInteractor;
    IInteractableInterface::FOnInteracted OnInteractedDelegate;

    void OpenKeypad(AActor* Interactor);
    void CloseKeypad();
    void BeginOpen();
    void SetPlayerInputMode(AActor* Interactor, bool bUIMode);
};