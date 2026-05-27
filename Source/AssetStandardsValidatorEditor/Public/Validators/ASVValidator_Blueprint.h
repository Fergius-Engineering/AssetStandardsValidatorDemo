// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVTypes.h"
#include "ASVValidator.h"

#include "ASVValidator_Blueprint.generated.h"

/** Base: all blueprint validation logic, empty CDO (all checks off). */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Blueprint"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_BlueprintBase : public UASVValidator
{
    GENERATED_BODY()
public:
    UASVValidator_BlueprintBase();

    /** Verify that boolean variables start with the 'b' prefix (e.g. bIsEnabled, bCanJump). This is the standard UE naming convention for booleans. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Bool Prefix")
    bool bCheckBoolPrefix = false;

    /** Severity reported when a boolean variable is missing the 'b' prefix. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Bool Prefix", meta = (EditCondition = "bCheckBoolPrefix"))
    EASVPriority BoolPrefixPriority = EASVPriority::P1;

    /** Verify that boolean variables named as questions use the 'bIs/bHas/bCan/bShould' pattern (e.g. bIsAlive, not bAlive). Improves readability. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Bool Is Pattern")
    bool bCheckBoolIsPattern = false;

    /** Severity reported when a boolean variable name does not follow the bIs/bHas/bCan pattern. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Bool Is Pattern", meta = (EditCondition = "bCheckBoolIsPattern"))
    EASVPriority BoolIsPatternPriority = EASVPriority::P2;

    /** Verify that variable names use PascalCase (e.g. PlayerHealth, not playerHealth or player_health). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Pascal Case")
    bool bCheckPascalCase = false;

    /** Severity reported when a variable name does not use PascalCase. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Pascal Case", meta = (EditCondition = "bCheckPascalCase"))
    EASVPriority PascalCasePriority = EASVPriority::P1;

    /** Verify that variables do not use C++ atomic type names directly (e.g. Float, Integer32). Use descriptive names instead (e.g. Speed, Count). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Atomic Type Name")
    bool bCheckAtomicTypeName = false;

    /** Severity reported when a variable has an atomic type name as its identifier. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Atomic Type Name", meta = (EditCondition = "bCheckAtomicTypeName"))
    EASVPriority AtomicTypeNamePriority = EASVPriority::P2;

    /** Verify that all EditAnywhere/BlueprintReadWrite variables have a tooltip set. Tooltips are essential for designer-facing Blueprints. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Editable Tooltips")
    bool bCheckEditableTooltips = false;

    /** Severity reported when an editable variable is missing a tooltip. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Editable Tooltips", meta = (EditCondition = "bCheckEditableTooltips"))
    EASVPriority EditableTooltipPriority = EASVPriority::P2;

    /** Verify that variables do not use the Config flag unless intentional. Config variables are read from .ini files and can cause unexpected behavior if added by mistake. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Config Flag")
    bool bCheckConfigFlag = false;

    /** Severity reported when a variable has the Config flag set unexpectedly. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Config Flag", meta = (EditCondition = "bCheckConfigFlag"))
    EASVPriority ConfigFlagPriority = EASVPriority::P1;

    /** Verify that numeric variables exposed to designers have ClampMin/ClampMax or UIMin/UIMax metadata set. Prevents invalid values from being entered in the editor. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Editable Ranges")
    bool bCheckEditableRanges = false;

    /** Severity reported when an editable numeric variable has no range constraints defined. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Editable Ranges", meta = (EditCondition = "bCheckEditableRanges"))
    EASVPriority EditableRangePriority = EASVPriority::P3;

    /** Verify that Blueprints with many editable variables organize them into categories. Uncategorized variables in large Blueprints make the Details panel hard to use. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Uncategorized Vars")
    bool bCheckUncategorizedVars = false;

    /** Severity reported when the number of uncategorized variables exceeds the threshold. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Uncategorized Vars", meta = (EditCondition = "bCheckUncategorizedVars"))
    EASVPriority UncategorizedVarsPriority = EASVPriority::P3;

    /** Number of uncategorized editable variables allowed before a violation is reported. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Uncategorized Vars", meta = (ClampMin = "1", UIMin = "3", UIMax = "20", EditCondition = "bCheckUncategorizedVars"))
    int32 UncategorizedVarThreshold = 5;

    /** Verify that variable names do not repeat the Blueprint class name as a prefix (e.g. PlayerCharacter.PlayerCharacterHealth -> PlayerCharacter.Health). Reduces name redundancy. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Redundant Context")
    bool bCheckRedundantContext = false;

    /** Severity reported when a variable name contains redundant context from the class name. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Redundant Context", meta = (EditCondition = "bCheckRedundantContext"))
    EASVPriority RedundantContextPriority = EASVPriority::P2;

    /** Verify that array variable names are plural (e.g. Enemies, not Enemy). Makes it obvious at a glance that the variable holds multiple values. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Array Plural")
    bool bCheckArrayPlural = false;

    /** Severity reported when an array variable has a singular name. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Array Plural", meta = (EditCondition = "bCheckArrayPlural"))
    EASVPriority ArrayPluralPriority = EASVPriority::P2;

    /** Verify that variable names do not include the type in the name (e.g. HealthFloat -> Health). Type information is already visible in the editor. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Complex Type Name")
    bool bCheckComplexTypeName = false;

    /** Severity reported when a variable name includes its type. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Complex Type Name", meta = (EditCondition = "bCheckComplexTypeName"))
    EASVPriority ComplexTypeNamePriority = EASVPriority::P2;

    /** Verify that Blueprints compile without errors or warnings. Compilation failures block the project from packaging. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Compilation")
    bool bCheckCompilation = false;

    /** Severity reported for Blueprint compilation errors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Compilation", meta = (EditCondition = "bCheckCompilation"))
    EASVPriority CompilationErrorPriority = EASVPriority::P0;

    /** Severity reported for Blueprint compilation warnings. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint|Compilation", meta = (EditCondition = "bCheckCompilation"))
    EASVPriority CompileWarningPriority = EASVPriority::P2;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual FText GetDisplayName_Implementation() const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual TArray<FName> GetRuleIDs() const override
    {
        return { "bp_bool_no_prefix", "bp_bool_is_pattern", "bp_var_not_pascal_case",
                 "bp_var_atomic_type_name", "bp_editable_missing_tooltip", "bp_var_config_flag",
                 "bp_editable_missing_range", "var_array_not_plural", "var_missing_type_name",
                 "bp_var_redundant_context", "bp_vars_uncategorized", "blueprint_compile_error" };
    }
};

/** Blueprint validator pre-configured with Allar's UE5 Style Guide variable and compilation rules. */
UCLASS(meta=(DisplayName="Blueprint (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_Blueprint : public UASVValidator_BlueprintBase
{
    GENERATED_BODY()
public:
    UASVValidator_Blueprint();
};
