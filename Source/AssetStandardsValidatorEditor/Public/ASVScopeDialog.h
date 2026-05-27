// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UASVEditorSubsystem;

class SASVScopeDialog : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SASVScopeDialog) {}
        SLATE_ARGUMENT(TWeakObjectPtr<UASVEditorSubsystem>, SubsystemRef)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    static void OpenAsWindow(TWeakObjectPtr<UASVEditorSubsystem> SubsystemRef);

private:
    FReply OnValidateClicked();
    FReply OnCloseClicked();

    TSharedRef<SWidget> BuildSummaryBar() const;
    TSharedRef<SWidget> BuildTree() const;
    FText ComputeAssetCountText() const;

    TWeakObjectPtr<UASVEditorSubsystem> SubsystemRef;
    TSharedPtr<SWindow> OwningWindow;
};
