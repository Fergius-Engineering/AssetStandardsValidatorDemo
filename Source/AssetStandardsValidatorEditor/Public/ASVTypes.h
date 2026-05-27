// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTypes.generated.h"

/** Priority buckets for validation results */
UENUM(BlueprintType)
enum class EASVPriority : uint8
{
	P0 = 0 UMETA(DisplayName = "P0 - Critical"),
	P1     UMETA(DisplayName = "P1 - Error"),
	P2     UMETA(DisplayName = "P2 - Warning"),
	P3     UMETA(DisplayName = "P3 - Info")
};

/**
 * Per-class rule: naming prefix and target folders for assets of a given class.
 * Used by naming and folder validators.
 */
USTRUCT(BlueprintType)
struct ASSETSTANDARDSVALIDATOREDITOR_API FASVClassRule
{
	GENERATED_BODY()

	/** Optional: pick a native class to match against (uses IsChildOf for Blueprints) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (AllowAbstract = "true", ToolTip = "Optional: pick a native class; matching uses IsChildOf for Blueprint parents."))
	TSubclassOf<UObject> ClassPicker = nullptr;

	/** Asset class name as reported by AssetRegistry (e.g. Blueprint, StaticMesh, Texture2D) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (DisplayName = "Asset Class Name"))
	FString AssetClassName;

	/** Preferred naming prefix for this class (e.g. BP_, SM_, T_) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
	FString Prefix;

	/** Required naming suffix for this class (e.g. _Decal, _SC). Empty = no suffix required. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
	FString Suffix;

	/** Set of allowed suffixes — asset must end with one of these. Empty = unconstrained. Takes priority over Suffix. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
	TArray<FString> AllowedSuffixes;

	/** Allowed target folders relative to scan roots (e.g. UI, Materials/Instances). Empty = unrestricted. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders", meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> TargetFolders;
};

/** A single validation result produced by a validator */
USTRUCT(BlueprintType)
struct ASSETSTANDARDSVALIDATOREDITOR_API FASVValidationResult
{
	GENERATED_BODY()

	/** Rule identifier (e.g. "missing_prefix", "wrong_folder") */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FName RuleID;

	/** Severity of this violation */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	EASVPriority Severity = EASVPriority::P2;

	/** Full object path of the asset */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString AssetPath;

	/** Asset class name */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString AssetClassName;

	/** Class name from the matched ClassPicker-based ClassRule (e.g. "CGHPlayerGameplayAbility").
	    Empty when no ClassPicker rule matched. */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString MatchedClassName;

	/** Package path (folder) of the asset */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString PackagePath;

	/** Human-readable problem description */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString Problem;

	/** Suggested fix description */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString Suggestion;

	/** Whether auto-fix is available for this violation */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	bool bAutoFixAvailable = false;

	/** Suggested new name (for rename auto-fix) */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString ProposedName;

	/** Suggested new path (for move auto-fix) */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString ProposedPath;

	/** Human-readable description of the auto-fix action (e.g. "Set sRGB = false") */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString ProposedFix;

	/** Multiple valid fix targets — validator populates when the answer is ambiguous. Best option first. */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	TArray<FString> FixChoices;

	/** Written by the caller (panel/batch/CLI) before invoking ExecuteAutoFix. Holds the user-selected choice. */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString SelectedFixChoice;

	/** FolderStructure sets this when a path picker should be offered alongside FixChoices. */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	bool bAllowFolderBrowse = false;

	/** Source identifier for variable/property fixes (e.g. variable name "myVar" for BP rename) */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString SourceIdentifier;

	/** UPROPERTY name on the validator that controls this rule (e.g. "DefaultClassPrefixes", "bCheckSRGB") */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString ConfigPropertyName;

	/** Name of the validator class that produced this result (e.g. "ASVValidator_NamingConvention") */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString ValidatorName;

	/** Name of the checker DataAsset config that ran this validator */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString CheckerName;

	/** Name of the trigger class that initiated validation (e.g. "ASVTrigger_OnSave") */
	UPROPERTY(BlueprintReadOnly, Category = "Result")
	FString TriggerName;
};

/** Aggregated report from running validation */
USTRUCT(BlueprintType)
struct ASSETSTANDARDSVALIDATOREDITOR_API FASVAuditReport
{
	GENERATED_BODY()

	/** All individual violations */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	TArray<FASVValidationResult> Results;

	/** Total number of assets scanned */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 AssetsScanned = 0;

	// demo builds only: total assets in scope before the 200-asset cap; 0 = cap not applied
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 OriginalAssetCount = 0;

	/** Violation counts by priority */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 P0Count = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 P1Count = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 P2Count = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Report")
	int32 P3Count = 0;

	/** Count per issue code */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	TMap<FString, int32> PerIssue;

	/** Hotspot folders (path -> issue count) */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	TMap<FString, int32> Hotspots;

	/** Scan roots used */
	UPROPERTY(BlueprintReadOnly, Category = "Report")
	TArray<FString> ScanRoots;

	/** Total issue count */
	int32 GetTotalIssues() const { return P0Count + P1Count + P2Count + P3Count; }

	/** Add a result and update summary counters. Silently drops duplicates (same AssetPath + RuleID). */
	void AddResult(const FASVValidationResult& Result)
	{
		const FString Key = Result.AssetPath + TEXT("|") + Result.RuleID.ToString();
		bool bAlreadyInSet = false;
		SeenKeys.Add(Key, &bAlreadyInSet);
		if (bAlreadyInSet)
		{
			return;
		}

		Results.Add(Result);
		PerIssue.FindOrAdd(Result.RuleID.ToString())++;
		if (!Result.PackagePath.IsEmpty())
		{
			Hotspots.FindOrAdd(Result.PackagePath)++;
		}
		switch (Result.Severity)
		{
		case EASVPriority::P0: ++P0Count; break;
		case EASVPriority::P1: ++P1Count; break;
		case EASVPriority::P2: ++P2Count; break;
		case EASVPriority::P3: ++P3Count; break;
		}
	}

private:
	/** Runtime dedup key set — not serialized, not a UPROPERTY */
	TSet<FString> SeenKeys;

public:

	/** Merge another report into this one */
	void Append(const FASVAuditReport& Other)
	{
		for (const FASVValidationResult& R : Other.Results)
		{
			AddResult(R);
		}
		AssetsScanned += Other.AssetsScanned;
	}
};
