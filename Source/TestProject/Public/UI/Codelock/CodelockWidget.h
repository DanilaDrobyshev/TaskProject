/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodelockWidget.generated.h"

class ADoorWithCodelock;


UCLASS()
class TESTPROJECT_API UCodelockWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    void Init(ADoorWithCodelock* Door, const int32 InCodeLength = 4);
    void Reset();

protected:

    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
    virtual bool NativeSupportsKeyboardFocus() const override { return true; }

    // Blueprint рисует цифру на нужной позиции
    UFUNCTION(BlueprintImplementableEvent)
    void OnDigitEntered(int32 Index, int32 Digit);

    // Blueprint стирает последнюю цифру
    UFUNCTION(BlueprintImplementableEvent)
    void OnDigitDeleted(int32 Index);

    UFUNCTION(BlueprintImplementableEvent)
    void OnWrongPassword();

    UFUNCTION(BlueprintImplementableEvent)
    void OnPasswordAccepted();

    UFUNCTION(BlueprintImplementableEvent)
    void OnReset();

private:

    TWeakObjectPtr<ADoorWithCodelock> OwningDoor;
    TArray<int32>                     Digits;
    int32                             CurrentIndex = 0;
    int32                             CodeLength = 4;

    void InputDigit(const int32 Digit);
    void DeleteDigit();
    void Submit();
};