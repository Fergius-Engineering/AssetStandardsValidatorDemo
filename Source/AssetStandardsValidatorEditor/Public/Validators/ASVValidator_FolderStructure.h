// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVTypes.h"
#include "ASVValidator.h"

#include "ASVValidator_FolderStructure.generated.h"

/** Base: all folder structure logic, empty CDO. */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Folder Structure"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_FolderStructureBase : public UASVValidator
{
    GENERATED_BODY()
public:
    UASVValidator_FolderStructureBase();

    /** Verify that each asset is stored in the correct folder according to ClassRules. For example, Static Meshes should be in a Meshes/ subfolder. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Wrong Folder")
    bool bCheckWrongFolder = false;

    /** Severity reported when an asset is found outside its expected folder. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Wrong Folder", meta = (EditCondition = "bCheckWrongFolder"))
    EASVPriority FolderPriority = EASVPriority::P0;

    /** Rules mapping asset classes to required folder paths. Each rule defines which folder a class of asset must reside in. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Wrong Folder", meta = (EditCondition = "bCheckWrongFolder"))
    TArray<FASVClassRule> ClassRules;

    /** Verify that folder names use PascalCase (e.g. MyAssets, not my_assets or myassets). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Pascal Case")
    bool bCheckPascalCase = false;

    /** Severity reported when a folder name does not use PascalCase. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Pascal Case", meta = (EditCondition = "bCheckPascalCase"))
    EASVPriority PascalCasePriority = EASVPriority::P1;

    /** Verify that folder names do not contain spaces. Spaces in folder paths can break command-line tools and some pipeline integrations. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Spaces")
    bool bCheckSpaces = false;

    /** Severity reported when a folder name contains a space character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Spaces", meta = (EditCondition = "bCheckSpaces"))
    EASVPriority SpacesPriority = EASVPriority::P0;

    /** Verify that folder names use only ASCII characters. Non-ASCII folder names can cause issues on certain operating systems and source control systems. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Unicode")
    bool bCheckUnicode = false;

    /** Severity reported when a folder name contains non-ASCII characters. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Unicode", meta = (EditCondition = "bCheckUnicode"))
    EASVPriority UnicodePriority = EASVPriority::P0;

    /** Verify that folder names are not on the disallowed list. Prevents temporary or reserved folder names from reaching the project. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Disallowed Names")
    bool bCheckDisallowedNames = false;

    /** Severity reported when a folder has a disallowed name. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Disallowed Names", meta = (EditCondition = "bCheckDisallowedNames"))
    EASVPriority DisallowedNamePriority = EASVPriority::P1;

    /** Folder names that are not permitted (case-insensitive). Example: ["temp", "test", "delete_me"]. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Disallowed Names", meta = (EditCondition = "bCheckDisallowedNames"))
    TArray<FString> DisallowedFolderNames;

    /** Verify that assets are not placed inside the Developers/ folder. Assets in Developers/ are personal workspaces and should not be committed to shared content. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Developers")
    bool bCheckDevelopersFolder = false;

    /** Severity reported when an asset is found inside the Developers/ folder. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Folders|Developers", meta = (EditCondition = "bCheckDevelopersFolder"))
    EASVPriority DevelopersFolderPriority = EASVPriority::P2;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool SupportsUndo() const override { return false; }
    virtual FText GetDisplayName_Implementation() const override;
    virtual TArray<FName> GetRuleIDs() const override
    {
        return { "wrong_folder", "folder_contains_spaces", "folder_contains_unicode",
                 "folder_not_pascal_case", "folder_disallowed_name", "folder_in_developers" };
    }

    TArray<FString> ResolveTargetFoldersForAsset(const FAssetData& AD) const;

protected:
    static FString NormalizeDir(const FString& In);

private:
    static bool IsUnderAnyRooted(const FString& PackagePath, const TArray<FString>& ScanRoots, const TArray<FString>& RequiredRelFolders);
    static bool RuleMatchesAsset(const FASVClassRule& Rule, const FAssetData& AD);
    FString ResolveMatchedClassNameForAsset(const FAssetData& AD) const;
    static TArray<FString> FindMatchingFoldersInRegistry(const FString& RelFolder, const FString& CurrentPkgPath, const TArray<FString>& ScanRoots, int32 MaxResults = 3);
};

/** Folder structure validator pre-configured with Allar's UE5 Style Guide folder rules. */
UCLASS(meta=(DisplayName="Folder Structure (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_FolderStructure : public UASVValidator_FolderStructureBase
{
    GENERATED_BODY()
public:
    UASVValidator_FolderStructure();
};
