// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AssetRegistry/AssetData.h"
#include "ASVTypes.h"
#include "ASVValidator.generated.h"

/**
 * Base class for all ASV validators.
 * Each validator checks one specific aspect (naming, folders, textures etc.).
 * Instances live inside UASVCheckerConfig DataAssets as subobjects.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator : public UObject
{
	GENERATED_BODY()

public:
	/** Whether this validator is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validator")
	bool bEnabled = true;

	/** Default severity for violations produced by this validator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validator")
	EASVPriority DefaultSeverity = EASVPriority::P1;

	/** Limit this validator to specific asset class names. Empty = applies to all. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Validator|Filter")
	TArray<FString> AppliesTo;

	/** Run validation on a single asset. Override to implement check logic. */
	UFUNCTION(BlueprintNativeEvent, Category = "Validator")
	void Validate(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const;
	virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const {}

	/** Check if this validator can auto-fix the given result. */
	UFUNCTION(BlueprintNativeEvent, Category = "Validator")
	bool CanAutoFix(const FASVValidationResult& Result) const;
	virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const { return false; }

	/** Execute auto-fix for the given result. Returns true if fix was applied. */
	UFUNCTION(BlueprintNativeEvent, Category = "Validator")
	bool ExecuteAutoFix(const FASVValidationResult& Result) const;
	virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const { return false; }

	/** Check if this validator applies to the given asset (based on AppliesTo filter) */
	UFUNCTION(BlueprintCallable, Category = "Validator")
	bool AppliesToAsset(const FAssetData& AssetData) const;

	/** Get a display name for this validator */
	UFUNCTION(BlueprintNativeEvent, Category = "Validator")
	FText GetDisplayName() const;
	virtual FText GetDisplayName_Implementation() const;

	/** false = auto-fix is a disk-level operation (rename/move/delete) and cannot participate in UE undo */
	virtual bool SupportsUndo() const { return true; }

	/** Returns all rule IDs this validator can produce. Used to pre-populate CB filters before validation runs. */
	virtual TArray<FName> GetRuleIDs() const { return {}; }
};
