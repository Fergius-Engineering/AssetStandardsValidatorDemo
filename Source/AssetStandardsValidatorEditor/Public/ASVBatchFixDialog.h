// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "Containers/Ticker.h"
#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

#include "ASVTypes.h"

class UASVEditorSubsystem;

enum class EASVBatchItemStatus : uint8 { Pending, Conflict, Fixed, Skipped };

struct FASVBatchFixItem
{
    TSharedPtr<bool> bEnabled;
    FASVValidationResult Result;
    FString WasText;
    FString WillBeText;
    FString FolderGroup;
    bool bConflict = false;
    FString ConflictReason;
    EASVBatchItemStatus Status = EASVBatchItemStatus::Pending;
    bool bUserOverride = false;   // user typed a custom WillBe — skip ambiguous choice dialog
    bool bUndoable = true;        // false = fix is a disk-level op (rename/move/delete), Ctrl+Z won't work
};

DECLARE_DELEGATE_FourParams(FOnASVBatchComplete, int32 /*Fixed*/, int32 /*Skipped*/, int32 /*Total*/, int32 /*RedirectorCount*/);
DECLARE_DELEGATE_OneParam(FOnASVFixesDone, TArray<FASVValidationResult> /*Fixed results*/);

class ASSETSTANDARDSVALIDATOREDITOR_API SASVBatchFixDialog : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SASVBatchFixDialog) {}
        SLATE_ARGUMENT(TArray<TSharedPtr<FASVBatchFixItem>>, Items)
        SLATE_EVENT(FOnASVBatchComplete, OnBatchComplete)
        SLATE_ARGUMENT(TWeakObjectPtr<UASVEditorSubsystem>, SubsystemRef)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    virtual ~SASVBatchFixDialog();

    static TSharedRef<SWindow> OpenAsWindow(
        TArray<TSharedPtr<FASVBatchFixItem>> Items,
        FOnASVBatchComplete InOnBatchComplete,
        FOnASVFixesDone InOnFixesDone,
        TWeakObjectPtr<UASVEditorSubsystem> InSubsystemRef);

    int32 GetFixedCount() const { return FixedCount; }
    TArray<FASVValidationResult> GetFixedResults() const { return FixedResults; }
    bool IsExecuting() const { return Phase == EPhase::Executing; }

private:
    enum class EPhase : uint8 { Idle, Executing, Done };

    int32 GetCheckedCount() const;
    void UpdateConfirmButton();
    void OnSearchChanged(const FText& Text);
    void OnSelectAllClicked();
    void OnDeselectAllClicked();
    void SetGroupEnabled(const FString& FolderGroup, bool bNewState);
    ECheckBoxState GetGroupCheckState(const FString& FolderGroup) const;
    void ToggleGroupCollapsed(const FString& FolderGroup);
    void OnConfirmClicked();
    bool OnFixTick(float DeltaTime);
    void FinishExecution();

    /** Returns false if user cancelled. Resolves SelectedFixChoice for all items that have FixChoices.Num() > 1. */
    bool ResolveAmbiguousChoices();

    /** Maps group key (RuleID+FixChoices) → chosen value. Persists across groups within one batch run. */
    TMap<FString, FString> ResolvedGroupChoices;
    TSharedRef<ITableRow> GenerateRow(TSharedPtr<FASVBatchFixItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

    TArray<TSharedPtr<FASVBatchFixItem>> AllItems;
    TArray<TSharedPtr<FASVBatchFixItem>> FilteredItems;
    TSet<FString> CollapsedGroups;
    FString CurrentFilter;
    TSharedPtr<SListView<TSharedPtr<FASVBatchFixItem>>> ListView;
    TSharedPtr<class SHeaderRow> HeaderRow;
    TSharedPtr<SWidget> ConfirmButtonLabel;
    TSharedPtr<SWidget> SearchBox;
    TSharedPtr<SWidget> ConfirmButton;
    TSharedPtr<SWidget> CancelButton;

    // for FTicker execution
    TArray<TSharedPtr<FASVBatchFixItem>> PendingQueue;
    FTSTicker::FDelegateHandle TickerHandle;

    EPhase Phase = EPhase::Idle;
    TSharedPtr<bool> bDialogIdle = MakeShared<bool>(true);
    int32 EditingRowCount = 0;

    int32 FixedCount = 0;
    int32 TotalToFix = 0;
    int32 SkippedCount = 0;

    TArray<FASVValidationResult> FixedResults;

    FOnASVBatchComplete OnBatchComplete;
    TWeakObjectPtr<UASVEditorSubsystem> SubsystemRef;
    TSharedPtr<SWindow> OwningWindow;
    TSharedPtr<SWidget> SummaryLabel;
    bool bHideFixed = false;
    TUniquePtr<class FScopedTransaction> BatchTransaction;
};
