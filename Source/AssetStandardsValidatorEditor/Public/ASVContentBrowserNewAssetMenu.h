// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

class UASVCheckerConfig;

enum class EASVCheckerPreset : uint8
{
    Empty,
    StarterTemplate,
    Allar,
};

class FASVContentBrowserNewAssetMenu
{
public:
    /** Register the "Asset Standards Validator" section in ContentBrowser.AddNewContextMenu */
    static void Register();

    /** Unregister — call from ShutdownModule */
    static void Unregister();

    /** Fill an existing UASVCheckerConfig with the chosen preset. Does NOT save to disk. */
    static void PopulateCheckerConfig(UASVCheckerConfig* Config, EASVCheckerPreset Preset);

private:
    static FString GetSelectedFolderPath();
    static void OnCreateConfig(EASVCheckerPreset Preset);
    static void OnValidateFolder();
};
