// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STreeView.h"
#include "ASVTypes.h"

struct FASVResultItem
{
    FASVValidationResult Result;
    bool bIsDemoFooter  = false;
    bool bIsAssetHeader = false;
    int32 HeaderIssueCount = 0;
};

struct FASVFilterState
{
    TSet<FName> Severities;   // empty = all pass; values: Error, Warning, Info
    TSet<FName> AssetTypes;   // empty = all pass
    TSet<FName> AssetClasses; // empty = all pass; values: matched ClassPicker class names
    TSet<FName> Rules;        // empty = all pass
    TSet<FName> Folders;      // empty = all pass
};

struct FASVDropdownData
{
    TSet<FName>* Selection = nullptr;
    TArray<TSharedPtr<FString>> AllOpts;
    TArray<TSharedPtr<FString>> VisibleOpts;
    FString DropSearch;
    TSharedPtr<SListView<TSharedPtr<FString>>> ListView;
    FString Label;
};

struct FFolderTreeNode
{
    FString SegmentName;
    FString FullPath;
    TArray<TSharedPtr<FFolderTreeNode>> Children;
    int32 AssetCount = 0;
};

class SASVPanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SASVPanel) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    ~SASVPanel();
    void SetResults(const FASVAuditReport& Report);
    void ClearResults();

    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
    virtual bool SupportsKeyboardFocus() const override { return true; }
    int32 GetFilteredCount() const
    {
#ifdef ASV_DEMO
        return FMath::Max(0, FilteredResults.Num() - 1);
#else
        return FilteredResults.Num();
#endif
    }
    int32 GetTotalCount() const { return AllResults.Num(); }
    void ApplySearchText(const FString& Text);
    bool FocusAsset(const FString& AssetPath);
    void FilterToFolder(const FString& InternalPath);
    void RemoveBatchFixedResults(TArray<FASVValidationResult> Fixed);

    /** Weak ref to the live panel — set in Construct, auto-clears on destruction */
    static TWeakPtr<SASVPanel> ActiveInstance;

private:
    TSharedRef<ITableRow> GenerateRow(TSharedPtr<FASVResultItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
    void RefreshFilteredList(bool bResetSelection = false);
    void OnValidationComplete(const FASVAuditReport& Report);
    void OnAssetResultsCleared(const FString& AssetPath);
    void OnResultsUpdated(const TSet<FString>& AffectedAssets, const TArray<FASVValidationResult>& NewResults);
    void OnResultDoubleClicked(TSharedPtr<FASVResultItem> Item);
    void UpdateStatusBar();

    FReply OnValidateAllClicked();
    FReply OnClearClicked();
    FReply OnExportClicked();

    void OnSearchTextChanged(const FText& NewText);
    void ParseSearchText(const FString& Input, FString& OutType, FString& OutRule, FString& OutValidator, FString& OutClass, FString& OutAsset, FString& OutFreeText) const;

    void OnSortModeChanged(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type NewSortMode);
    EColumnSortMode::Type GetSortModeForColumn(const FName& ColumnName) const;

    void RebuildDropdownOptions();
    TSharedRef<SWidget> MakeFilterDropdown(FASVDropdownData* Data);
    TSharedRef<ITableRow> GenerateDropdownRow(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& Owner, FASVDropdownData* Data);
    TSharedRef<SWidget> MakeFolderTreeDropdown();
    TSharedRef<ITableRow> GenerateFolderTreeRow(TSharedPtr<FFolderTreeNode> Node, const TSharedRef<STableViewBase>& Owner);
    void RebuildFolderTree(const TSet<FString>& DirectFolders, const TMap<FString, int32>& DirectCounts);

    static FName GetAssetType(const FASVValidationResult& R);
    static FName GetTopFolder(const FASVValidationResult& R);

    FReply OnScopeClicked();

    void OnResultSelectionChanged(TSharedPtr<FASVResultItem> Item, ESelectInfo::Type SelectType);
    void OnContentBrowserSelectionChanged(const TArray<FAssetData>& SelectedAssets, bool bIsPrimaryBrowser);
    void UpdateDetailPanel();
    FReply OnNavigateSelectedClicked();
    FReply OnFixSelectedClicked();
    void ShowFolderBrowseForResult(TSharedPtr<FASVResultItem> Item);
    void ClearSelection();
    bool HasUniformFixableSelection() const;
    void UpdateBatchFixCache();
    FReply OnBatchFixClicked();
    bool   CanBatchFix() const;

    TSharedRef<SWidget> MakeIgnoredListMenu();
    FReply OnRemoveIgnoredAsset(FString AssetPath);
    FReply OnRemoveIgnoredFolder(FString FolderPath);

    TSharedPtr<class SComboButton> IgnoredComboButton;
    FText  GetBatchFixTooltip() const;
    TSharedPtr<SWidget> OnContextMenuOpening();
    void CopySelectedResultToClipboard();

    TArray<TSharedPtr<FASVResultItem>> AllResults;
    TArray<TSharedPtr<FASVResultItem>> FilteredResults;
    TSharedPtr<SListView<TSharedPtr<FASVResultItem>>> ResultListView;
    TSharedPtr<SWidget> EmptyOverlay;
    TSharedPtr<STextBlock> StatusText;
    int32 LastAssetsScanned = 0;
    int32 DemoOriginalAssetCount = 0;

    FASVFilterState FilterState;

    FString SearchText;
    FString SearchFreeText;
    TSharedPtr<class SSearchBox> SearchBox;

    FName SortColumn = TEXT("Severity");
    EColumnSortMode::Type SortMode = EColumnSortMode::Ascending;

    TArray<TSharedPtr<FASVResultItem>> SelectedItems;
    TSharedPtr<FASVResultItem> PrimaryItem;
    /** When >= 0, RefreshFilteredList will select the item at this index after rebuild (used to advance selection after fix/auto-rename). */
    int32 PendingSelectIndex = INDEX_NONE;
    /** Identifies the fixed row so we know when it has actually disappeared from FilteredResults. */
    FString PendingSelectAssetPath;
    FName PendingSelectRuleID;
    TSharedPtr<class SBorder> DetailBorder;
    TSharedPtr<SWidget> DetailContent;
    TSharedPtr<class SBorder> DetailGridBorder;
    bool bDetailExpanded = true;

    FASVDropdownData SevData;
    FASVDropdownData TypeData;
    FASVDropdownData ClassData;
    FASVDropdownData RuleData;
    FASVDropdownData FolderData;

    TArray<TSharedPtr<FFolderTreeNode>> FolderRoots;
    TSharedPtr<STreeView<TSharedPtr<FFolderTreeNode>>> FolderTreeView;
    TSet<FString> ExpandedFolderPaths;

    bool bGroupByAsset = false;

    // cached per RefreshFilteredList — avoids per-frame allocation from Slate attribute polls
    bool bCanBatchFix = false;
    FText CachedBatchFixTooltip;

    TSharedPtr<SHeaderRow> ResultHeaderRow;
};
