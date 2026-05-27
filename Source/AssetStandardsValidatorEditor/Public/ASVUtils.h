// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"
#include "UObject/SoftObjectPath.h"
#include "ASVCompat.h"
#include "ASVTypes.h"

namespace ASV_ClassUtils
{
	/**
	 * Try to resolve the parent class path of an asset (Blueprints included) without loading;
	 * falls back to loading GeneratedClass if needed.
	 * Returns true and fills OutParentClassPath (e.g. "/Script/Engine.Actor") on success.
	 */
	static bool TryGetParentClassPath(const FAssetData& AD, FString& OutParentClassPath)
	{
		// Fast paths: rely on AssetRegistry tags first (no loading).
		FString Str;

		// Modern tag used by BP assets
		if (AD.GetTagValue(FName(TEXT("ParentClassPath")), Str) && !Str.IsEmpty())
		{
			OutParentClassPath = Str;
			return true;
		}

		// Older tag sometimes present on BPs (direct parent — more specific than native ancestor)
		if (AD.GetTagValue(FName(TEXT("ParentClass")), Str) && !Str.IsEmpty())
		{
			OutParentClassPath = Str;
			return true;
		}

		// Native ancestor fallback (less specific, used only when direct parent tag absent)
		if (AD.GetTagValue(FName(TEXT("NativeParentClassPath")), Str) && !Str.IsEmpty())
		{
			OutParentClassPath = Str;
			return true;
		}

		// Fallback: only "GeneratedClass" is present -> we may need to load to get SuperClass.
		if (AD.GetTagValue(FName(TEXT("GeneratedClass")), Str) && !Str.IsEmpty())
		{
			const FSoftClassPath GeneratedClassPath(Str);

			UClass* GeneratedCls = GeneratedClassPath.ResolveClass();
			if (!GeneratedCls)
			{
				GeneratedCls = GeneratedClassPath.TryLoadClass<UObject>();
			}

			if (GeneratedCls)
			{
				if (UClass* Super = GeneratedCls->GetSuperClass())
				{
					OutParentClassPath = Super->GetPathName();
					return true;
				}
			}
		}

		return false;
	}
}

// severity → color palette shared across overlay icons, folder badges, and tooltip cards
namespace ASVColors
{
    // icon / background accent: P0/P1 red · P2 yellow · P3 blue
    inline FLinearColor IconColor(EASVPriority S)
    {
        if (S <= EASVPriority::P1) return FLinearColor(0.90f, 0.15f, 0.15f, 1.f);
        if (S == EASVPriority::P2) return FLinearColor(1.00f, 0.75f, 0.00f, 1.f);
        return                            FLinearColor(0.40f, 0.65f, 1.00f, 1.f);
    }

    // left accent strip on error cards
    inline FLinearColor AccentColor(EASVPriority S)
    {
        if (S <= EASVPriority::P1) return FLinearColor(0.75f, 0.21f, 0.17f, 1.f);
        if (S == EASVPriority::P2) return FLinearColor(0.72f, 0.57f, 0.04f, 1.f);
        return                            FLinearColor(0.34f, 0.43f, 0.62f, 1.f);
    }

    // card background (very dark tint)
    inline FLinearColor CardBgColor(EASVPriority S)
    {
        if (S <= EASVPriority::P1) return FLinearColor(0.12f, 0.07f, 0.07f, 1.f);
        if (S == EASVPriority::P2) return FLinearColor(0.11f, 0.10f, 0.04f, 1.f);
        return                            FLinearColor(0.12f, 0.12f, 0.15f, 1.f);
    }

    // badge text color
    inline FLinearColor BadgeTextColor(EASVPriority S)
    {
        if (S <= EASVPriority::P1) return FLinearColor(0.87f, 0.44f, 0.44f, 1.f);
        if (S == EASVPriority::P2) return FLinearColor(0.78f, 0.66f, 0.20f, 1.f);
        return                            FLinearColor(0.53f, 0.63f, 0.82f, 1.f);
    }

    // badge pill background
    inline FLinearColor BadgeBgColor(EASVPriority S)
    {
        if (S <= EASVPriority::P1) return FLinearColor(0.29f, 0.12f, 0.12f, 1.f);
        if (S == EASVPriority::P2) return FLinearColor(0.23f, 0.19f, 0.06f, 1.f);
        return                            FLinearColor(0.17f, 0.17f, 0.24f, 1.f);
    }

    // "P0" / "P1" / "P2" / "P3" label string
    inline FString Label(EASVPriority S)
    {
        switch (S)
        {
        case EASVPriority::P0: return TEXT("P0");
        case EASVPriority::P1: return TEXT("P1");
        case EASVPriority::P2: return TEXT("P2");
        default:               return TEXT("P3");
        }
    }
}