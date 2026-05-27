// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "ASVLog.h"
#include "ASVGlobalSettings.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnASVContentBrowserSettingsChanged);

/**
 * Global plugin settings (Project Settings -> Plugins -> Asset Standards Validator).
 * Only contains plugin-wide settings. Per-checker rules live in UASVCheckerConfig DataAssets.
 */
UCLASS(config = AssetStandardsValidator, defaultconfig, meta = (DisplayName = "Asset Standards Validator"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVGlobalSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UASVGlobalSettings();
    virtual void PostInitProperties() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual FName GetContainerName() const override { return FName("Project"); }
    virtual FName GetCategoryName() const override  { return FName("Plugins"); }
    virtual FName GetSectionName() const override   { return FName("AssetStandardsValidator"); }
#endif

    /** Content roots to scan. Empty = /Game only (default). Add paths to scan additional roots. */
    UPROPERTY(Config, EditAnywhere, Category = "Scan", meta = (RelativeToGameContentDir, LongPackageName))
    TArray<FDirectoryPath> ScanRoots;

    /** Content roots to exclude from scanning */
    UPROPERTY(Config, EditAnywhere, Category = "Scan", meta = (RelativeToGameContentDir, LongPackageName))
    TArray<FDirectoryPath> ExcludeScanRoots;

    /** Scan subfolders of each root */
    UPROPERTY(Config, EditAnywhere, Category = "Scan")
    bool bRecursiveScan = true;

    /** Wildcard patterns to ignore during scanning (e.g. Developers/*, ThirdParty/*) */
    UPROPERTY(Config, EditAnywhere, Category = "Scan")
    TArray<FString> IgnorePaths;

    /** Exact asset package paths excluded from all validation (e.g. /Game/Characters/SK_Hero) */
    UPROPERTY(Config, EditAnywhere, Category = "Scan")
    TArray<FString> IgnoreAssets;

    /** Folder paths excluded from all validation — recursive (e.g. /Game/ThirdPartyArt) */
    UPROPERTY(Config, EditAnywhere, Category = "Scan")
    TArray<FString> IgnoreFolders;

    /** Open the Message Log automatically when issues are found */
    UPROPERTY(Config, EditAnywhere, Category = "Output")
    bool bOpenMessageLogOnIssues = false;

    /** Enable verbose logging for all ASV categories. Useful for diagnosing validation issues. */
    UPROPERTY(Config, EditAnywhere, Category = "Output")
    bool bVerboseLogging = false;

    /** Maximum number of example violations to display in logs */
    UPROPERTY(Config, EditAnywhere, Category = "Output", meta = (ClampMin = "0"))
    int32 LogExamplesLimit = 50;

    /** Maximum assets to process in a batch (0 = unlimited). Use to prevent long validation runs. */
    UPROPERTY(Config, EditAnywhere, Category = "Batch", meta = (ClampMin = "0"))
    int32 MaxBatchSize = 0;

    /** Show red dot on asset thumbnails when issues are found. Disable if overlays slow down large projects. */
    UPROPERTY(Config, EditAnywhere, Category = "Content Browser")
    bool bShowAssetBadges = true;

    /** Show red dot on folders when any contained asset has P0/P1 issues. UE 5.3+ only. */
    UPROPERTY(Config, EditAnywhere, Category = "Content Browser")
    bool bShowFolderBadges = true;

    static FOnASVContentBrowserSettingsChanged OnContentBrowserSettingsChanged;

    /** Get scan roots as normalized string array */
    static TArray<FString> GetScanRootsStr();

    /** Normalize a directory path: ensure leading /, remove trailing / */
    static FString NormalizeDir(const FString& In);

    /** Returns true if PackagePath is in IgnoreAssets (exact) or under any IgnoreFolders entry (prefix).
     *  Pass Settings explicitly for tests; nullptr uses the project CDO. */
    static bool IsIgnoredAsset(const FString& PackagePath, const UASVGlobalSettings* Settings = nullptr);

    /** Updates an IgnoreAssets array when an asset is renamed/moved. Returns true if an entry was updated.
     *  Caller is responsible for calling SaveConfig() when operating on the CDO. */
    static bool UpdateIgnoredAssetPath(TArray<FString>& InOutIgnoreAssets, const FString& OldPath, const FString& NewPath);
};
