/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "Core/TestCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Interfaces/Interact/InteractableInterface.h>

ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SetInputEnabled(false);
	
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction,     ETriggerEvent::Started,   this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,	   ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started,   this, &ATestCharacter::Interact);
		EnhancedInputComponent->BindAction(MoveAction,     ETriggerEvent::Triggered, this, &ATestCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,     ETriggerEvent::Triggered, this, &ATestCharacter::Look);
	}
}

void ATestCharacter::SetInputEnabled(bool bEnabled)
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!IsValid(PC)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!IsValid(Subsystem)) return;

	if (bEnabled)
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	else
		Subsystem->RemoveMappingContext(DefaultMappingContext);
}

void ATestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (!IsValid(Controller)) return;
	
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
	
}

void ATestCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!IsValid(Controller)) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATestCharacter::Interact()
{
	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 60.f);
	const FVector End = Start + GetActorForwardVector() * 300.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) return;

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor)) return;

	if (!HitActor->Implements<UInteractableInterface>()) return;

	if (IInteractableInterface::Execute_CanInteract(HitActor, this))
		IInteractableInterface::Execute_OnInteract(HitActor, this);
}