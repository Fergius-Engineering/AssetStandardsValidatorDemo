// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVTypes.h"
#include "ASVValidator.h"

#include "ASVValidator_AssetHealth.generated.h"

/** Base: all asset health logic, empty CDO (all checks off). */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Asset Health"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_AssetHealthBase : public UASVValidator
{
    GENERATED_BODY()
public:
    UASVValidator_AssetHealthBase();

    /** Verify that the project has no stale asset redirectors. Redirectors are left behind when assets are moved or renamed; they waste disk space and slow down the cooker if not fixed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AssetHealth|Redirectors")
    bool bCheckRedirectors = false;

    /** Severity reported when a stale redirector is found. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AssetHealth|Redirectors", meta = (EditCondition = "bCheckRedirectors"))
    EASVPriority RedirectorPriority = EASVPriority::P1;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool SupportsUndo() const override { return false; }
    virtual FText GetDisplayName_Implementation() const override;
    virtual TArray<FName> GetRuleIDs() const override { return { "stale_redirector" }; }
};

/** Asset health validator pre-configured with Allar's UE5 Style Guide (redirectors on). */
UCLASS(meta=(DisplayName="Asset Health (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_AssetHealth : public UASVValidator_AssetHealthBase
{
    GENERATED_BODY()
public:
    UASVValidator_AssetHealth();
};
