/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInputComponent;

UCLASS()
class TESTPROJECT_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	ATestCharacter();

	void SetInputEnabled(bool bEnabled);	

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMissionCompleteAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Movement input handle
	void Move(const FInputActionValue& Value);

	// Look input handle
	void Look(const FInputActionValue& Value);

	virtual void Interact();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
