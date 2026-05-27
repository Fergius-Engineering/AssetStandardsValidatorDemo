// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "AssetRegistry/AssetData.h"
#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "UObject/ObjectSaveContext.h"
#include "Widgets/SWidget.h"

#include "ASVLog.h"
#include "ASVTypes.h"
#include "ASVEditorSubsystem.generated.h"

class FASVFrontendFilter_HasError;
class FASVFrontendFilter_Rule;
class FASVFrontendFilter_Severity;
class FContentBrowserModule;
class FFrontendFilter;
class FFrontendFilterCategory;
class UASVCheckerConfig;
class UASVTrigger;
class UASVValidator;
struct FASVBatchFixItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASVValidationComplete, const FASVAuditReport& /*Report*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASVAssetRemoved, const FString& /*AssetPath*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnASVResultsUpdated, const TSet<FString>& /*AffectedAssets*/, const TArray<FASVValidationResult>& /*NewResults*/);

/**
 * Central orchestrator for the Asset Standards Validator.
 * Discovers checker configs, activates triggers, runs validators, and caches results.
 */
UCLASS()
class ASSETSTANDARDSVALIDATOREDITOR_API UASVEditorSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    FOnASVValidationComplete OnValidationComplete;
    FOnASVAssetRemoved OnAssetResultsCleared;
    FOnASVResultsUpdated OnResultsUpdated;

    const FASVAuditReport& GetLastReport() const { return LastReport; }

    /** Tracked by SASVPanel — true while at least one panel instance is alive */
    bool bASVPanelAlive = false;

    // CB frontend filter support
    bool bValidationHasEverRun = false;

    void GetASVFilters(TArray<TSharedRef<FFrontendFilter>>& OutFilters);

    /** Pointer to issues for one asset, or nullptr if none cached. Public for CB filter access. */
    const TArray<FASVValidationResult>* GetIssuesForAsset(FName AssetPathName) const;

    /** Set true during batch fix execution — suppresses per-asset trigger revalidation */
    bool bBatchFixInProgress = false;

    UFUNCTION(BlueprintCallable, Category = "ASV")
    TArray<FASVValidationResult> GetCachedResultsAsArray() const;

    /** Scan AssetRegistry for all UASVCheckerConfig DataAssets and activate them */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    TArray<UASVCheckerConfig*> DiscoverCheckers();

    /** Get currently active checkers */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    TArray<UASVCheckerConfig*> GetActiveCheckers() const;

    /** Run all active checkers against all scannable assets */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    FASVAuditReport RunFullAudit();

    /** Run a specific checker against the given assets */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    FASVAuditReport RunChecker(UASVCheckerConfig* Checker, const TArray<FAssetData>& Assets);

    /** Run all active checkers against the given assets */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    FASVAuditReport RunAllCheckers(const TArray<FAssetData>& Assets);

    /** Run all active checkers against a subset of assets, merging results into LastReport incrementally */
    FASVAuditReport RunAllCheckersIncremental(const TArray<FAssetData>& Assets);

    /** Run specific checkers by name */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    FASVAuditReport RunCheckersByName(const TArray<FName>& CheckerNames, const TArray<FAssetData>& Assets);

    /** Collect all assets matching global scan settings */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    TArray<FAssetData> CollectAssetsForScan();

    /** Validate all assets in a specific folder path */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    FASVAuditReport ValidateFolder(FString FolderPath);

    /** Execute auto-fix for a validation result. Wraps in FScopedTransaction, re-validates after fix. */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    bool ExecuteAutoFix(const FASVValidationResult& Result, bool bSilent = false);

    /** pre-check: fills bConflict + ConflictReason on each item */
    void PreCheckBatchFix(TArray<TSharedPtr<FASVBatchFixItem>>& Items) const;

    /** Remove specific issues from cache after a batch fix — faster than RunFullAudit */
    void RemoveBatchFixedFromCache(const TArray<FASVValidationResult>& Fixed);

    /** Clear cached issues for a set of assets and notify the panel (used when assets are added to the ignore list) */
    void RemoveIssuesForAssets(const TArray<FAssetData>& Assets);

    /** Re-validate a single asset across all active checkers. Updates LastReport and broadcasts. */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    void RevalidateSingleAsset(const FString& AssetPath);

    /** Emit report to the Message Log with clickable actions */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    void EmitToMessageLog(const FASVAuditReport& Report);

    /** C++-only overload — bSuppressOpen=true skips ML.Open() even if setting is on */
    void EmitToMessageLog(const FASVAuditReport& Report, bool bSuppressOpen);

    /** Total cached issue count across all assets */
    int32 GetTotalIssueCount() const;

    /** True if the cache has at least one auto-fixable issue for this asset */
    bool HasFixableIssuesForAsset(FName AssetPathName) const;

    /** Emit report summary to UE Log */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    void EmitToUELog(const FASVAuditReport& Report);

    /** Create the default checker DataAsset in the plugin Content folder */
    UFUNCTION(BlueprintCallable, Category = "ASV")
    UASVCheckerConfig* CreateDefaultChecker();

    /** Called when any trigger fires — public for test access */
    void OnTriggerFired(UASVCheckerConfig* Checker, const TArray<FAssetData>& Assets, const FString& TriggerName = FString());

    /** Returns names of validator types shared by A and B when their path filters overlap.
     *  Empty = no conflict. Public for testing. */
    static TArray<FString> DetectValidatorOverlap(const UASVCheckerConfig* A, const UASVCheckerConfig* B);

private:
    FASVAuditReport LastReport;

    /** Active checker configs */
    UPROPERTY()
    TArray<TObjectPtr<UASVCheckerConfig>> ActiveCheckers;

    /** Cached validation issues, grouped by asset path for O(1) overlay lookup */
    TMap<FName /*AssetPath*/, TArray<FASVValidationResult>> IssuesByAsset;

    /** Whether a debounced redraw is already scheduled */
    bool bRedrawScheduled = false;

    /** Whether a cached-asset re-validation is already scheduled (after config save) */
    bool bCachedRevalidationScheduled = false;

    /** Last Content Browser path for refresh */
    FString LastContentBrowserPath;

    /** Upsert issue into the bucket for its asset (replaces existing issue with same RuleID) */
    void AddIssue(const FASVValidationResult& Result);

    /** Drop all cached issues for one asset. Returns how many were removed. */
    int32 RemoveIssuesForAsset(FName AssetPathName);

    /** Drop one specific issue identified by (asset, rule). Returns true if removed. */
    bool RemoveIssueByRule(FName AssetPathName, FName RuleID);

    /** Total cached issue count across all assets */
    int32 NumIssues() const;

    /** Add results to cache (deduplicates) and schedule a debounced redraw */
    void CacheResults(const TArray<FASVValidationResult>& Results);

    /** Clear all cached errors for validated assets (even if no errors found) */
    void ClearCacheForValidatedAssets(const TArray<FAssetData>& ValidatedAssets);

    /** Schedule debounced redraw tick */
    void ScheduleRedraw(float DelaySeconds = 0.3f);

    /** Debounced redraw callback */
    bool OnRedrawTick(float DeltaTime);

    /** Generate overlay widget for Content Browser asset thumbnail */
    TSharedRef<SWidget> GenerateAssetStateWidget(const FAssetData& AssetData);

    /** Called when an asset is removed from AssetRegistry */
    void OnAssetRemoved(const FAssetData& AssetData);

    /** Called when an asset is renamed — clears cached results for old path */
    void OnAssetRenamed(const FAssetData& AssetData, const FString& OldObjectPath);

    FDelegateHandle AssetRemovedHandle;
    FDelegateHandle AssetRenamedHandle;
    FDelegateHandle OverlayGeneratorHandle;
    FDelegateHandle ConfigSavedHandle;
    FDelegateHandle ContentBrowserSettingsHandle;

    /** Live asset tile overlay widgets — only holds currently visible tiles (~20-100) */
    TArray<TWeakPtr<SWidget>> AssetOverlayLiveWidgets;

    void RegisterAssetBadge(FContentBrowserModule& CB);
    void UnregisterAssetBadge(FContentBrowserModule& CB);

    /** Prune dead widget ptrs and invalidate all live overlay widgets */
    void InvalidateAssetOverlayWidgets();

    /** Build tooltip text from current cache for a given asset */
    FString BuildAssetTooltipText(FName AssetPathName) const;

    /** Fired when a checker config DataAsset is pre-saved — schedules cache re-validation */
    void OnCheckerConfigPreSaved(UObject* Object, FObjectPreSaveContext SaveContext);

    /** Ticker callback: runs RevalidateCachedAssets after config save debounce */
    bool OnCachedRevalidateTick(float DeltaTime);

    /** Re-validate only assets currently in IssuesByAsset against updated checker rules */
    void RevalidateCachedAssets();

    /** Bind trigger delegates for a checker */
    void BindTriggers(UASVCheckerConfig* Checker);

    /** Unbind all triggers */
    void UnbindAllTriggers();

    /** Check if an asset is ignored by global settings (uses both PackagePath for folder rules and PackageName for asset rules) */
    bool IsIgnoredByGlobalSettings(const FAssetData& AssetData) const;

    /** Find the validator instance that produced a result, using CheckerName + ValidatorName */
    UASVValidator* FindValidatorForResult(const FASVValidationResult& Result) const;

    /** Check all active config pairs for overlapping validators+paths. Logs a table.
     *  bShowToast=true on config save, false at startup. */
    void CheckAllConfigOverlaps(bool bShowToast = false) const;

    /** Schedule a deferred overlap check (runs after save+trigger chain completes) */
    void ScheduleOverlapCheck();

    /** Ticker callback for deferred overlap check */
    bool OnOverlapCheckTick(float DeltaTime);

    bool bOverlapCheckScheduled = false;

    // CB frontend filters — persistent objects, never recreated after init
    TSharedPtr<FFrontendFilterCategory>                        FilterCategory;
    TSharedPtr<FFrontendFilterCategory>                        RuleFilterCategory;
    TSharedPtr<FASVFrontendFilter_HasError>                   HasErrorFilter;
    TArray<TSharedPtr<FASVFrontendFilter_Severity>>           SeverityFilters;
    TMap<FName, TSharedPtr<FASVFrontendFilter_Rule>>          RuleFilterCache;
    bool                                                       bFilterBroadcastScheduled = false;

    void InitFilters();
    void RebuildRuleFilters();
    void ScheduleFilterBroadcast();
    bool OnFilterBroadcastTick(float);

};
