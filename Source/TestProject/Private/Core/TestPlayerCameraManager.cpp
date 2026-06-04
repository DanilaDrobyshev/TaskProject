/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Core/TestPlayerCameraManager.h"
#include "Core/TestCharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ATestPlayerCameraManager::ATestPlayerCameraManager()
{
    Phase = ECameraPhase::FPS;
}

void ATestPlayerCameraManager::StartIntroSequence()
{
    FVector EyeLoc; FRotator EyeRot;
    GetPawnEyesPOV(EyeLoc, EyeRot);

    IntroStartLocation = EyeLoc + EyeRot.RotateVector(CinematicStartLocation);
    IntroStartRotation = (EyeLoc - IntroStartLocation).Rotation();

    bMeshHidden = false;
    SetCharacterInputEnabled(false);
    SetPhase(ECameraPhase::Cinematic);
}

void ATestPlayerCameraManager::StartOutroSequence()
{
    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot)) return;

    OutroEndLocation = EyeLoc + EyeRot.RotateVector(OutroOffset);
    OutroEndRotation = (EyeLoc - OutroEndLocation).Rotation();

    bMeshHidden = false;
    bMeshShownForOutro = false;
    SetCharacterInputEnabled(false);
    SetPhase(ECameraPhase::OutroFlyOut);
}

void ATestPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
    switch (Phase)
    {
    case ECameraPhase::Cinematic:   TickCinematic(OutVT, DeltaTime); break;
    case ECameraPhase::FadeOut:     TickFadeOut(OutVT, DeltaTime); break;
    case ECameraPhase::FadeIn:      TickFadeIn(OutVT, DeltaTime); break;
    case ECameraPhase::FPS:         TickFPS(OutVT, DeltaTime); break;
    case ECameraPhase::OutroFlyOut: TickOutroFlyOut(OutVT, DeltaTime); break;
    case ECameraPhase::OutroFlyIn:  TickOutroFlyIn(OutVT, DeltaTime); break;
    }
}

void ATestPlayerCameraManager::TickCinematic(FTViewTarget& OutVT, float DeltaTime)
{
    const float Alpha = TickAlpha(DeltaTime, CinematicDuration);
    const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, kEaseExp);

    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot)) return;

    OutVT.POV.Location = FMath::Lerp(IntroStartLocation, EyeLoc, EasedAlpha);
    OutVT.POV.Rotation = FMath::Lerp(IntroStartRotation, EyeRot, EasedAlpha);
    OutVT.POV.FOV = FMath::Lerp(IntroFOV, GameFOV, EasedAlpha);

    if (Alpha >= kFadeStartAlpha)
    {
        const float FadeProgress = FMath::Clamp(
            (Alpha - kFadeStartAlpha) / (kPhaseCompleteAlpha - kFadeStartAlpha), 0.f, 1.f);
        SetManualCameraFade(FMath::Pow(FadeProgress, 1.f / FadeCoefficient), FLinearColor::Black, false);
    }

    if (Alpha >= kMeshHideAlpha && !bMeshHidden)
    {
        bMeshHidden = true;
        HidePawnMesh(true);
    }

    if (Alpha >= kPhaseCompleteAlpha)
        SetPhase(ECameraPhase::FadeIn);
}

void ATestPlayerCameraManager::TickFadeOut(FTViewTarget& OutVT, float DeltaTime)
{
    const float Alpha = TickAlpha(DeltaTime, FadeDuration);

    ApplyEyesPOV(OutVT, GameFOV);
    SetManualCameraFade(Alpha, FLinearColor::Black, false);

    if (Alpha >= kPhaseCompleteAlpha) SetPhase(ECameraPhase::FadeIn);
}

void ATestPlayerCameraManager::TickFadeIn(FTViewTarget& OutVT, float DeltaTime)
{
    const float Alpha = TickAlpha(DeltaTime, FadeDuration);
    const float FadeProgress = FMath::Clamp(Alpha * FadeCoefficient, 0.f, 1.f);

    ApplyEyesPOV(OutVT, GameFOV);
    SetManualCameraFade(1.f - FadeProgress, FLinearColor::Black, false);

    if (Alpha >= kPhaseCompleteAlpha)
    {
        StopCameraFade();
        SetPhase(ECameraPhase::FPS);
        SetCharacterInputEnabled(true);
    }
}

void ATestPlayerCameraManager::TickFPS(FTViewTarget& OutVT, float DeltaTime)
{
    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot))
    {
        Super::UpdateViewTarget(OutVT, DeltaTime);
        return;
    }
    ApplyEyesPOV(OutVT, GameFOV);
}

void ATestPlayerCameraManager::TickOutroFlyOut(FTViewTarget& OutVT, float DeltaTime)
{
    const float Alpha = TickAlpha(DeltaTime, OutroDuration);
    const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, kEaseExp);

    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot)) return;

    if (!bMeshShownForOutro)
    {
        bMeshShownForOutro = true;
        bMeshHidden = false;
        HidePawnMesh(false);
    }

    OutVT.POV.Location = FMath::Lerp(EyeLoc, OutroEndLocation, EasedAlpha);
    OutVT.POV.Rotation = FMath::Lerp(EyeRot, OutroEndRotation, EasedAlpha);
    OutVT.POV.FOV = GameFOV;

    if (Alpha <= OutroFadeStartAlpha)
    {
        const float FadeProgress = FMath::Clamp(1.f - Alpha / OutroFadeStartAlpha, 0.f, 1.f);
        SetManualCameraFade(FMath::Pow(FadeProgress, 1.f / OutroFadeCoefficient), FLinearColor::Black, false);
    }
    else
    {
        StopCameraFade();
    }

    if (Alpha >= kPhaseCompleteAlpha)
        SetPhase(ECameraPhase::OutroFlyIn);
}

void ATestPlayerCameraManager::TickOutroFlyIn(FTViewTarget& OutVT, float DeltaTime)
{
    const float Alpha = TickAlpha(DeltaTime, OutroDuration);
    const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, kEaseExp);

    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot)) return;

    OutVT.POV.Location = FMath::Lerp(OutroEndLocation, EyeLoc, EasedAlpha);
    OutVT.POV.Rotation = FMath::Lerp(OutroEndRotation, EyeRot, EasedAlpha);
    OutVT.POV.FOV = GameFOV;

    if (Alpha >= OutroFadeStartAlpha)
    {
        const float FadeProgress = FMath::Clamp(
            (Alpha - OutroFadeStartAlpha) / (kPhaseCompleteAlpha - OutroFadeStartAlpha), 0.f, 1.f);
        SetManualCameraFade(FMath::Pow(FadeProgress, 1.f / OutroFadeCoefficient), FLinearColor::Black, false);
    }

    if (Alpha >= OutroMeshHideAlpha && !bMeshHidden)
    {
        bMeshHidden = true;
        HidePawnMesh(true);
    }

    if (Alpha >= kPhaseCompleteAlpha)
    {
        StopCameraFade();
        SetPhase(ECameraPhase::FPS);
        SetCharacterInputEnabled(true);
    }
}

void ATestPlayerCameraManager::SetPhase(ECameraPhase NewPhase)
{
    Phase = NewPhase;
    PhaseElapsed = 0.f;
}

float ATestPlayerCameraManager::TickAlpha(float DeltaTime, float Duration)
{
    PhaseElapsed += DeltaTime;
    return FMath::Clamp(PhaseElapsed / Duration, 0.f, kPhaseCompleteAlpha);
}

bool ATestPlayerCameraManager::GetPawnEyesPOV(FVector& OutLocation, FRotator& OutRotation) const
{
    const APlayerController* PC = GetOwningPlayerController();
    if (!IsValid(PC)) return false;

    const APawn* Pawn = PC->GetPawn();
    if (!IsValid(Pawn)) return false;

    Pawn->GetActorEyesViewPoint(OutLocation, OutRotation);
    return true;
}

void ATestPlayerCameraManager::ApplyEyesPOV(FTViewTarget& OutVT, float FOV) const
{
    FVector EyeLoc; FRotator EyeRot;
    if (!GetPawnEyesPOV(EyeLoc, EyeRot)) return;

    OutVT.POV.Location = EyeLoc;
    OutVT.POV.Rotation = EyeRot;
    OutVT.POV.FOV = FOV;
}

void ATestPlayerCameraManager::SetCharacterInputEnabled(bool bEnabled)
{
    if (ATestCharacter* Character = Cast<ATestCharacter>(GetOwningPlayerController()->GetPawn()))
        Character->SetInputEnabled(bEnabled);
}