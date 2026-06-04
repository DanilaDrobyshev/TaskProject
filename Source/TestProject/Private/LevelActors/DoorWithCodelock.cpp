/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "LevelActors/DoorWithCodelock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include <UI/Codelock/CodelockWidget.h>

ADoorWithCodelock::ADoorWithCodelock()
{
    PrimaryActorTick.bCanEverTick = false;
    FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
    SetRootComponent(FrameMesh);

    HingePivot = CreateDefaultSubobject<USceneComponent>(TEXT("HingePivot"));
    HingePivot->SetupAttachment(FrameMesh);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(HingePivot);

    KeypadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("KeypadWidget"));
    KeypadWidgetComponent->SetupAttachment(FrameMesh);
    KeypadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    KeypadWidgetComponent->SetDrawSize(FVector2D(400.f, 300.f));
    KeypadWidgetComponent->SetVisibility(false);
}

void ADoorWithCodelock::BeginPlay()
{
    Super::BeginPlay();

    if (PasswordWidgetClass)
        KeypadWidgetComponent->SetWidgetClass(PasswordWidgetClass);
}

void ADoorWithCodelock::OnInteract_Implementation(AActor* Interactor)
{
    if (bIsOpen) return;

    CurrentInteractor = Interactor;
    OpenKeypad(Interactor);
}

bool ADoorWithCodelock::CanInteract_Implementation(AActor* Interactor) const
{
    return !bIsOpen;
}

void ADoorWithCodelock::OpenKeypad(AActor* Interactor)
{
    UCodelockWidget* Widget = Cast<UCodelockWidget>(KeypadWidgetComponent->GetUserWidgetObject());
    if (!IsValid(Widget)) return;

    Widget->Init(this, CorrectPassword.Len());
    KeypadWidgetComponent->SetVisibility(true);
    Widget->SetFocus();
}

void ADoorWithCodelock::CloseKeypad()
{
    KeypadWidgetComponent->SetVisibility(false);
    SetPlayerInputMode(CurrentInteractor.Get(), false);
}

void ADoorWithCodelock::BeginOpen()
{
    CloseKeypad();
    GetWorldTimerManager().SetTimer(
        OpenTimerHandle,
        [this]()
        {
            const float NewAngle = FMath::FInterpConstantTo(CurrentAngle, OpenAngle, GetWorld()->GetDeltaSeconds(), OpenSpeed);
            CurrentAngle = NewAngle;
            HingePivot->SetRelativeRotation(FRotator(0.f, CurrentAngle, 0.f));

            if (FMath::IsNearlyEqual(CurrentAngle, OpenAngle, 0.1f))
            {
                HingePivot->SetRelativeRotation(FRotator(0.f, OpenAngle, 0.f));
                bIsOpen = true;
                GetWorldTimerManager().ClearTimer(OpenTimerHandle);

                if (CurrentInteractor.IsValid())
                    OnInteractedDelegate.Broadcast(CurrentInteractor.Get());
            }
        },
        0.016f, true // ~60fps
    );

}

void ADoorWithCodelock::OnCorrectPassword()
{
    BeginOpen();
}

void ADoorWithCodelock::OnWrongPassword()
{
    
}

void ADoorWithCodelock::ClosePasswordWidget()
{
    CloseKeypad();
}


void ADoorWithCodelock::SetPlayerInputMode(AActor* Interactor, bool bUIMode)
{
    if (!IsValid(Interactor)) return;

    APlayerController* PC = Cast<APlayerController>(Interactor);

    if (!IsValid(PC)) return;

    if (bUIMode)
    {
        FInputModeGameAndUI Mode;
        Mode.SetWidgetToFocus(KeypadWidgetComponent->GetUserWidgetObject()->TakeWidget());
        PC->SetInputMode(Mode);
    }
    else
    {
        PC->SetInputMode(FInputModeGameOnly{});
    }

    PC->bShowMouseCursor = false;
}