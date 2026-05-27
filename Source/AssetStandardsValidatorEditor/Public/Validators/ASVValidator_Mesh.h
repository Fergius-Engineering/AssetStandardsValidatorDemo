// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVTypes.h"
#include "ASVValidator.h"

#include "ASVValidator_Mesh.generated.h"

UENUM(BlueprintType)
enum class EMeshNanitePolicy : uint8
{
    MustEnable UMETA(DisplayName = "Must Enable"),
    MustDisable UMETA(DisplayName = "Must Disable"),
    AllowAny UMETA(DisplayName = "Allow Any")
};

/** Base: all mesh validation logic, empty CDO (all checks off). */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Mesh"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_MeshBase : public UASVValidator
{
    GENERATED_BODY()
public:
    UASVValidator_MeshBase();

    /** Verify that static meshes have at least one collision shape defined. Meshes without collision will not block physics queries or traces at runtime. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Collision")
    bool bCheckCollision = false;

    /** Severity reported when a mesh has no collision shapes. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Collision", meta = (EditCondition = "bCheckCollision"))
    EASVPriority CollisionPriority = EASVPriority::P1;

    /** Verify that meshes with a high triangle count have LODs generated. LODs are required for performance — meshes above MinTriCountForLOD should have at least one reduced-detail LOD. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|LOD")
    bool bCheckLODs = false;

    /** Severity reported when a mesh above the triangle threshold has no LODs. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|LOD", meta = (EditCondition = "bCheckLODs"))
    EASVPriority LODPriority = EASVPriority::P2;

    /** Minimum triangle count that requires at least one LOD. Meshes with fewer triangles are exempt from the LOD check. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|LOD", meta = (EditCondition = "bCheckLODs", ClampMin = "100", UIMin = "100", UIMax = "50000"))
    int32 MinTriCountForLOD = 5000;

    /** Verify that Nanite is enabled or disabled on meshes according to NanitePolicy. Enforces consistent use of Nanite virtualized geometry across the project. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Nanite")
    bool bCheckNanite = false;

    /** Severity reported when a mesh does not satisfy the Nanite policy. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Nanite", meta = (EditCondition = "bCheckNanite"))
    EASVPriority NanitePriority = EASVPriority::P2;

    /** Whether high-poly meshes must have Nanite enabled, disabled, or either is acceptable. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Nanite", meta = (EditCondition = "bCheckNanite"))
    EMeshNanitePolicy NanitePolicy = EMeshNanitePolicy::MustEnable;

    /** Minimum triangle count for the Nanite policy to apply. Meshes below this threshold are exempt from the Nanite check. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh|Nanite", meta = (EditCondition = "bCheckNanite", ClampMin = "100", UIMin = "100", UIMax = "100000"))
    int32 MinTriCountForNanite = 10000;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual FText GetDisplayName_Implementation() const override;

private:
    bool HasCollision(const UStaticMesh* Mesh) const;
    int32 GetLODCount(const UStaticMesh* Mesh) const;
    virtual TArray<FName> GetRuleIDs() const override
    {
        return { "mesh_nanite_policy", "mesh_no_collision", "mesh_no_lods" };
    }

#if WITH_EDITOR
    bool HasNaniteEnabled(const UStaticMesh* Mesh) const;
#endif
};

/** Mesh validator pre-configured with Allar's UE5 Style Guide mesh quality rules. */
UCLASS(meta=(DisplayName="Mesh (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_Mesh : public UASVValidator_MeshBase
{
    GENERATED_BODY()
public:
    UASVValidator_Mesh();
};
