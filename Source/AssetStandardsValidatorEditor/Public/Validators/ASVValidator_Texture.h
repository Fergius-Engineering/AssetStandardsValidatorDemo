// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#include "ASVTypes.h"
#include "ASVValidator.h"

#include "ASVValidator_Texture.generated.h"

/** Base: all texture validation logic, empty CDO (all checks off). */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Texture"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_TextureBase : public UASVValidator
{
    GENERATED_BODY()
public:
    UASVValidator_TextureBase();

    /** Verify that texture dimensions are powers of two (e.g. 512, 1024, 2048). Non-POT textures cannot use mipmaps on all platforms and may waste GPU memory. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Power Of Two")
    bool bCheckPowerOfTwo = false;

    /** Severity reported when a texture has non-power-of-two dimensions. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Power Of Two", meta = (EditCondition = "bCheckPowerOfTwo"))
    EASVPriority PowerOfTwoPriority = EASVPriority::P1;

    /** Verify that texture dimensions do not exceed MaxTextureSize. Oversized textures increase memory usage and loading times. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Max Size")
    bool bCheckMaxSize = false;

    /** Severity reported when a texture exceeds the maximum allowed size. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Max Size", meta = (EditCondition = "bCheckMaxSize"))
    EASVPriority MaxSizePriority = EASVPriority::P0;

    /** Maximum allowed texture dimension in pixels. Textures wider or taller than this value will be flagged. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Max Size", meta = (EditCondition = "bCheckMaxSize", ClampMin = "1", UIMin = "64", UIMax = "16384"))
    int32 MaxTextureSize = 8192;

    /** Verify that color textures have sRGB enabled and non-color textures (normals, masks, data) have it disabled. Incorrect sRGB causes washed-out or overly dark colors at runtime. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|sRGB")
    bool bCheckSRGB = false;

    /** Severity reported when a texture has an incorrect sRGB setting for its type. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|sRGB", meta = (EditCondition = "bCheckSRGB"))
    EASVPriority SRGBPriority = EASVPriority::P1;

    /** Verify that textures use the correct compression format for their type. Wrong compression can cause visual artifacts or excessive memory usage. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Compression")
    bool bCheckCompression = false;

    /** Severity reported when a texture uses an unexpected compression format. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Compression", meta = (EditCondition = "bCheckCompression"))
    EASVPriority CompressionPriority = EASVPriority::P2;

    /** Verify that textures are assigned to the correct LOD group (Texture Group). Correct grouping ensures proper streaming and memory budgeting. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Texture Group")
    bool bCheckTextureGroup = false;

    /** Severity reported when a texture is in an unexpected LOD group. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Texture Group", meta = (EditCondition = "bCheckTextureGroup"))
    EASVPriority TextureGroupPriority = EASVPriority::P2;

    /** Map of texture name suffix -> expected LOD group name (e.g. "_N" -> "WorldNormalMap"). Suffix matching is case-insensitive. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Texture Group", meta = (EditCondition = "bCheckTextureGroup"))
    TMap<FString, FString> ExpectedTextureGroup;

    /** Asset class names excluded from all texture checks. Useful for skipping render targets or procedural textures that cannot follow standard rules. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture|Filter")
    TArray<FString> ExcludeClasses;

    virtual void Validate_Implementation(const FAssetData& AssetData, TArray<FASVValidationResult>& OutResults) const override;
    virtual FText GetDisplayName_Implementation() const override;
    virtual bool CanAutoFix_Implementation(const FASVValidationResult& Result) const override;
    virtual bool ExecuteAutoFix_Implementation(const FASVValidationResult& Result) const override;

protected:
    TMap<FString, bool> NonColorSuffixes;
    TMap<FString, bool> ColorSuffixes;
    TMap<FString, FString> ExpectedCompression;

    virtual TArray<FName> GetRuleIDs() const override
    {
        return { "texture_not_power_of_two", "texture_exceeds_max_size",
                 "texture_wrong_srgb", "texture_wrong_group", "texture_wrong_compression" };
    }

private:
    static bool IsPowerOfTwo(int32 N);
    FString DetectTextureSuffix(const FString& AssetName) const;
};

/** Texture validator pre-configured with Allar's UE5 Style Guide texture rules. */
UCLASS(meta=(DisplayName="Texture (Allar)"))
class ASSETSTANDARDSVALIDATOREDITOR_API UASVValidator_Texture : public UASVValidator_TextureBase
{
    GENERATED_BODY()
public:
    UASVValidator_Texture();
};
