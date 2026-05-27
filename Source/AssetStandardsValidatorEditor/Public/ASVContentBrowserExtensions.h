// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

#include "ASVTypes.h"

class FContentBrowserModule;
class UASVCheckerConfig;
struct FAssetData;
struct FContentBrowserItem;

/** One group of fixable results sharing the same validator type. */
struct FASVFixGroup
{
    FName ValidatorClass;       // key — ValidatorName from FASVValidationResult
    FText DisplayName;          // human-readable, e.g. "Naming Convention"
    TArray<FASVValidationResult> Results;
    bool bNeedsChoiceDialog = false;  // true if any result has FixChoices.Num() > 1 || bAllowFolderBrowse
};

class FASVContentBrowserExtensions
{
public:
    static void RegisterAll(FContentBrowserModule& CB);
    static void UnregisterAll(FContentBrowserModule& CB);

    static TArray<FASVFixGroup> BuildFixGroupsForFolders(
        const TArray<FASVValidationResult>& AllResults,
        const TArray<FString>& Paths);

private:
    static TSharedRef<FExtender> ExtendAssetMenu(const TArray<FAssetData>& Assets);
    static TSharedRef<FExtender> ExtendPathMenu(const TArray<FString>& Paths);

    static void ExecuteValidateAssets(TArray<FAssetData> Assets);
    static void ExecuteOpenInPanel(TArray<FAssetData> Assets);
    static void ExecuteValidateFolder(TArray<FString> Paths);

    // fix menu
    static FText GetValidatorDisplayName(const FString& ValidatorName);
    static FText MakeGroupLabel(const FASVFixGroup& Group);
    static FText MakeGroupTooltip(const FASVFixGroup& Group);
    static TArray<FASVFixGroup> BuildFixGroups(const TArray<FAssetData>& Assets);
    static TArray<FASVFixGroup> BuildFixGroupsForFolders(const TArray<FString>& Paths);
    static void ExecuteFixGroup(FASVFixGroup Group);

    static void ExecuteToggleAssetIgnore(TArray<FAssetData> Assets);
    static bool IsAnyAssetIgnored(const TArray<FAssetData>& Assets);

    static void ExecuteToggleFolderIgnore(TArray<FString> Paths);
    static bool IsAnyFolderIgnored(const TArray<FString>& Paths);

    static TArray<UASVCheckerConfig*> CollectConfigsForAssets(const TArray<FAssetData>& Assets);
    static void ExecuteOpenConfigs(TArray<FAssetData> Assets);

    static TSharedPtr<SWidget> GenerateFolderBadge(const FContentBrowserItem& Item);

    static TArray<FDelegateHandle> AssetExtenderHandles;
    static TArray<FDelegateHandle> PathExtenderHandles;

    static FDelegateHandle FolderIconGeneratorHandle;
    static FDelegateHandle ValidationCompleteHandle;
    static FDelegateHandle ResultsUpdatedHandle;
    static FDelegateHandle ResultsClearedHandle;
};
