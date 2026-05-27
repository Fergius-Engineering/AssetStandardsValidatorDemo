// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ASVTypes.h"
#include "ASVValidator.h"
#include "ASVTrigger.h"
#include "ASVCheckerConfig.generated.h"

// fires once per asset during RunValidation — used for progress reporting
DECLARE_DELEGATE_OneParam(FASVValidationProgress, const FAssetData& /*Current*/);

/**
 * Checker configuration DataAsset.
 * Holds validators (what to check), triggers (when to check), and filters (which assets).
 * Create via Content Browser → Miscellaneous → Data Asset → ASVCheckerConfig.
 */
UCLASS(BlueprintType, Blueprintable)
class ASSETSTANDARDSVALIDATOREDITOR_API UASVCheckerConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Display name for this checker */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checker")
	FName CheckerName;

	/** Whether this checker is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checker")
	bool bEnabled = true;

	/** Description of what this checker does */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checker", meta = (MultiLine = true))
	FString Description;

	/** Validation rules to run. Each validator checks a specific aspect (naming, folders, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Validators")
	TArray<TObjectPtr<UASVValidator>> Validators;

	/** When to run validation. Each trigger listens for a specific event (save, startup, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Triggers")
	TArray<TObjectPtr<UASVTrigger>> Triggers;

	/** Limit validation to these content paths. Empty = use global scan roots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filters", meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> AssetPathFilters;

	/** Limit validation to these asset class names. Empty = all classes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filters")
	TArray<FString> AssetClassFilters;

	/** Exclude assets under these paths. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filters", meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> ExcludePaths;

	/** Run all validators on the given assets (pre-filtered by this checker's filters). */
	UFUNCTION(BlueprintCallable, Category = "Checker")
	FASVAuditReport RunValidation(const TArray<FAssetData>& Assets) const;

	// C++ overload — OnProgress fires after every asset (including filtered-out) for progress UI
	FASVAuditReport RunValidation(const TArray<FAssetData>& Assets, const FASVValidationProgress& OnProgress) const;

	/** Check if an asset passes this checker's filters */
	UFUNCTION(BlueprintCallable, Category = "Checker")
	bool PassesFilters(const FAssetData& AssetData) const;

	/** Activate all triggers on this checker */
	void ActivateAllTriggers();

	/** Deactivate all triggers on this checker */
	void DeactivateAllTriggers();
};
