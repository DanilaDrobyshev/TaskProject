/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TestPlayerCameraManager.generated.h"

UENUM(BlueprintType)
enum class ECameraPhase : uint8
{
    Cinematic,
    FadeOut,
    FadeIn,
    FPS,
    OutroFlyOut,
    OutroFlyIn,
};

UCLASS()
class TESTPROJECT_API ATestPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    ATestPlayerCameraManager();

    void StartIntroSequence();
    void StartOutroSequence();

protected:
    virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro")
    FVector CinematicStartLocation = FVector(300.f, 0.f, 200.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro")
    float CinematicDuration = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|Fade")
    float FadeDuration = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|Fade")
    float kFadeStartAlpha = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|Fade")
    float FadeCoefficient = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|Fade")
    float kMeshHideAlpha = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|FOV")
    float IntroFOV = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro|FOV")
    float GameFOV = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outro")
    float OutroDuration = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outro")
    FVector OutroOffset = FVector(-300.f, 0.f, 100.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outro|Fade")
    float OutroFadeStartAlpha = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outro|Fade")
    float OutroFadeCoefficient = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outro|Fade")
    float OutroMeshHideAlpha = 0.8f;

private:
    static constexpr float kEaseExp = 2.f;
    static constexpr float kPhaseCompleteAlpha = 1.f;

    ECameraPhase Phase = ECameraPhase::FPS;
    float        PhaseElapsed = 0.f;

    FVector  IntroStartLocation = FVector::ZeroVector;
    FRotator IntroStartRotation = FRotator::ZeroRotator;
    FVector  OutroEndLocation = FVector::ZeroVector;
    FRotator OutroEndRotation = FRotator::ZeroRotator;

    bool bMeshHidden = false;
    bool bMeshShownForOutro = false;

    void TickCinematic(FTViewTarget& OutVT, float DeltaTime);
    void TickFadeOut(FTViewTarget& OutVT, float DeltaTime);
    void TickFadeIn(FTViewTarget& OutVT, float DeltaTime);
    void TickFPS(FTViewTarget& OutVT, float DeltaTime);
    void TickOutroFlyOut(FTViewTarget& OutVT, float DeltaTime);
    void TickOutroFlyIn(FTViewTarget& OutVT, float DeltaTime);

    void  SetPhase(ECameraPhase NewPhase);
    float TickAlpha(float DeltaTime, float Duration);
    bool  GetPawnEyesPOV(FVector& OutLocation, FRotator& OutRotation) const;
    void  ApplyEyesPOV(FTViewTarget& OutVT, float FOV) const;
    void  SetCharacterInputEnabled(bool bEnabled);

    FORCEINLINE void HidePawnMesh(bool bsHidden)
    {
        if (const APlayerController* PC = GetOwningPlayerController())
        {
            if (APawn* Pawn = PC->GetPawn())
            {
                Pawn->SetActorHiddenInGame(bsHidden);
            }
        }
    }
};