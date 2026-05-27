// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/IAssetRegistry.h"

// UE 5.1 introduced FTopLevelAssetPath, FAssetData::AssetClassPath, FARFilter::ClassPaths
// UE 5.0 uses FName-based AssetClass, ClassNames

#define ASV_UE_5_1_OR_LATER (ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1))

// UE 5.7 deprecated UStaticMesh::NaniteSettings direct access; use GetNaniteSettings() / SetNaniteSettings()
#define ASV_UE_5_7_OR_LATER (ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 7))

#define ASV_UE_5_3_OR_LATER (ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3))

/** Get the asset class name as FString from FAssetData (compat 5.0/5.1+) */
inline FString ASV_GetAssetClassName(const FAssetData& AD)
{
#if ASV_UE_5_1_OR_LATER
	return AD.AssetClassPath.GetAssetName().ToString();
#else
	return AD.AssetClass.ToString();
#endif
}

/** Get asset by object path string (compat 5.0/5.1+) */
inline FAssetData ASV_GetAssetByObjectPath(IAssetRegistry& Registry, const FString& Path)
{
#if ASV_UE_5_1_OR_LATER
	return Registry.GetAssetByObjectPath(FSoftObjectPath(Path));
#else
	return Registry.GetAssetByObjectPath(FName(*Path));
#endif
}

/** Get asset object path as FName for fast cache lookup (compat 5.0/5.1+) */
inline FName ASV_GetAssetPathName(const FAssetData& AD)
{
#if ASV_UE_5_1_OR_LATER
	return FName(*AD.GetSoftObjectPath().ToString());
#else
	return AD.ObjectPath;
#endif
}

/** Add class filter to FARFilter (compat 5.0/5.1+) */
inline void ASV_AddClassFilter(FARFilter& Filter, UClass* Class)
{
#if ASV_UE_5_1_OR_LATER
	Filter.ClassPaths.Add(Class->GetClassPathName());
#else
	Filter.ClassNames.Add(Class->GetFName());
#endif
}
