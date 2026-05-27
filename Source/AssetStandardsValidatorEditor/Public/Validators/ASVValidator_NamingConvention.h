// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVValidator.h"
#include "ASVTypes.h"
#include "ASVValidator_NamingConvention.generated.h"

/**
 * Base class: all naming convention logic, empty CDO.
 * Use this when building your own naming rules from scratch.
 */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Naming Convention"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_NamingConventionBase : public UASVValidator
{
    GENERATED_BODY()

public:
    UASVValidator_NamingConventionBase();

    /** Verify that each asset has the correct type prefix (e.g. SM_ for Static Mesh, T_ for Texture). Uses DefaultClassPrefixes and ClassRules to determine expected prefix. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Prefix")
    bool bCheckPrefix = false;

    /** Severity reported when an asset is missing or has a wrong prefix. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Prefix", meta = (EditCondition = "bCheckPrefix"))
    EASVPriority PrefixPriority = EASVPriority::P0;

    /** Map of UClass name -> expected prefix (e.g. "StaticMesh" -> "SM_"). Used when no ClassRule matches the asset. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Prefix", meta = (EditCondition = "bCheckPrefix"))
    TMap<FString, FString> DefaultClassPrefixes;

    /** Per-class rules that override DefaultClassPrefixes. Each rule can target a specific class and define required prefix, suffix, and allowed folder. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Prefix", meta = (EditCondition = "bCheckPrefix"))
    TArray<FASVClassRule> ClassRules;

    /** Verify that each asset has the correct type suffix where applicable (e.g. _Inst for Material Instances). Uses DefaultClassSuffixes. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Suffix")
    bool bCheckSuffix = false;

    /** Severity reported when an asset is missing or has a wrong suffix. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Suffix", meta = (EditCondition = "bCheckSuffix"))
    EASVPriority SuffixPriority = EASVPriority::P1;

    /** Map of UClass name -> expected suffix (e.g. "MaterialInstanceConstant" -> "_Inst"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Suffix", meta = (EditCondition = "bCheckSuffix"))
    TMap<FString, FString> DefaultClassSuffixes;

    TMap<FString, TArray<FString>> DefaultClassAllowedSuffixes;

    /** Verify that assets only use suffixes from the allowed list for their class. Prevents unrecognized or mismatched suffixes. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Allowed Suffix")
    bool bCheckAllowedSuffix = false;

    /** Severity reported when an asset has a suffix not in the allowed list for its class. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Allowed Suffix", meta = (EditCondition = "bCheckAllowedSuffix"))
    EASVPriority AllowedSuffixPriority = EASVPriority::P1;

    /** Verify that asset names match a custom regular expression pattern. Useful for enforcing studio-specific naming conventions beyond prefix/suffix. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Name Pattern")
    bool bCheckNamePattern = false;

    /** Severity reported when an asset name does not match NamePattern. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Name Pattern", meta = (EditCondition = "bCheckNamePattern"))
    EASVPriority PatternPriority = EASVPriority::P1;

    /** Regular expression that asset names must match. Leave empty to disable the pattern check. Example: ^[A-Z][A-Za-z0-9_]+$ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Name Pattern", meta = (EditCondition = "bCheckNamePattern"))
    FString NamePattern;

    /** Verify that each word segment in the asset name starts with an uppercase letter (PascalCase), e.g. T_Cross not T_cross. Applies to the base name after stripping the expected prefix. Skipped when the prefix is missing (to avoid double-flagging). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Case")
    bool bCheckPascalCase = false;

    /** Severity reported when an asset name segment starts with a lowercase letter. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Case", meta = (EditCondition = "bCheckPascalCase"))
    EASVPriority PascalCasePriority = EASVPriority::P2;

    /** Verify that asset names use only ASCII characters. Non-ASCII characters can cause issues on case-sensitive file systems and in some pipeline tools. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Non ASCII")
    bool bCheckNonAsciiName = false;

    /** Severity reported when an asset name contains non-ASCII characters. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Non ASCII", meta = (EditCondition = "bCheckNonAsciiName"))
    EASVPriority NonAsciiPriority = EASVPriority::P1;

    /** Verify that asset names are not placeholder or temporary names. Checks DisallowSubstrings and DisallowedNames lists. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Junk Name")
    bool bCheckJunkName = false;

    /** Severity reported when an asset name matches a junk pattern. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Junk Name", meta = (EditCondition = "bCheckJunkName"))
    EASVPriority JunkNamePriority = EASVPriority::P1;

    /** Asset names containing any of these substrings will be flagged. Example: ["test", "temp", "placeholder", "delete"]. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Junk Name", meta = (EditCondition = "bCheckJunkName"))
    TArray<FString> DisallowSubstrings;

    /** Exact asset names that are not allowed. Example: ["NewTexture", "New Blueprint"]. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming|Junk Name", meta = (EditCondition = "bCheckJunkName"))
    TArray<FString> DisallowedNames;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool SupportsUndo() const override { return false; }
    virtual FText GetDisplayName_Implementation() const override;
    virtual TArray<FName> GetRuleIDs() const override
    {
        return { "missing_prefix", "duplicate_prefix", "missing_suffix", "missing_allowed_suffix",
                 "junk_name", "wrong_case", "non_ascii_name", "name_pattern_mismatch" };
    }

    FString ResolvePrefixForAsset(const FAssetData& AD) const;
    FString ResolveSuffixForAsset(const FAssetData& AD) const;
    TArray<FString> ResolveAllowedSuffixesForAsset(const FAssetData& AD) const;

private:
    static bool RuleMatchesAsset(const FASVClassRule& Rule, const FAssetData& AD);
    static FString BuildProposedName(const FString& CurrentName, const FString& ExpectedPrefix);
    static FString BuildProposedNameWithSuffix(const FString& CurrentName, const FString& ExpectedPrefix, const FString& ExpectedSuffix);
    static FString ResolveMatchedClassNameForAsset(const TArray<FASVClassRule>& Rules, const FAssetData& AD);
};

/**
 * Naming convention validator pre-configured with Allar's UE5 Style Guide rules.
 * All prefixes/suffixes/rules from https://github.com/Allar/ue5-style-guide
 */
UCLASS(meta=(DisplayName="Naming Convention (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_NamingConvention : public UASVValidator_NamingConventionBase
{
    GENERATED_BODY()
public:
    UASVValidator_NamingConvention();
};
