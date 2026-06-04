/*Copyright(c) 2026 Danila Drobyshev
This source code is provided solely for evaluation and review as part of a recruitment process.
You may view and analyze the source code.
You may not use, copy, modify, distribute, publish, sublicense, or create derivative works from this code, in whole or in part, for any purpose.
All rights reserved.*/


#include "UI/Codelock/CodelockWidget.h"
#include "LevelActors/DoorWithCodelock.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"


static const TMap<FKey, int32> KeyToDigit = {
    { EKeys::Zero,  0 }, { EKeys::One,   1 }, { EKeys::Two,   2 },
    { EKeys::Three, 3 }, { EKeys::Four,  4 }, { EKeys::Five,  5 },
    { EKeys::Six,   6 }, { EKeys::Seven, 7 }, { EKeys::Eight, 8 },
    { EKeys::Nine,  9 }
};


void UCodelockWidget::Init(ADoorWithCodelock* Door, const int32 InCodeLength)
{
    OwningDoor = Door;
    CodeLength = FMath::Clamp(InCodeLength, 1, 8);
    Reset();
}

void UCodelockWidget::Reset()
{
    Digits.Init(-1, CodeLength);
    CurrentIndex = 0;
    OnReset();
}

void UCodelockWidget::NativeConstruct()
{
    Super::NativeConstruct();
    Reset();
}

FReply UCodelockWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    const FKey Key = InKeyEvent.GetKey();

    if (const int32* Digit = KeyToDigit.Find(Key))
    {
        InputDigit(*Digit);
        return FReply::Handled();
    }

    if (Key == EKeys::BackSpace)
    {
        DeleteDigit();
        return FReply::Handled();
    }

    if (Key == EKeys::Escape)
    {
        if (OwningDoor.IsValid()) OwningDoor->ClosePasswordWidget();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCodelockWidget::InputDigit(const int32 Digit)
{
    if (CurrentIndex >= CodeLength) return;

    Digits[CurrentIndex] = Digit;
    OnDigitEntered(CurrentIndex, Digit);
    ++CurrentIndex;

    // Автосабмит когда все цифры введены
    if (CurrentIndex == CodeLength) Submit();
}

void UCodelockWidget::DeleteDigit()
{
    if (CurrentIndex <= 0) return;

    --CurrentIndex;
    Digits[CurrentIndex] = -1;
    OnDigitDeleted(CurrentIndex);
}

void UCodelockWidget::Submit()
{
    if (!OwningDoor.IsValid()) return;

    FString Entered;
    Entered.Reserve(CodeLength);
    for (const int32 D : Digits) Entered += FString::FromInt(D);

    if (Entered == OwningDoor->GetCorrectPassword())
    {
        OnPasswordAccepted();
        OwningDoor->OnCorrectPassword();
    }
    else
    {
        OnWrongPassword();
        Reset();
    }
}