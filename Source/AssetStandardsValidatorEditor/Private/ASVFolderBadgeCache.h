// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTypes.h"

class SWidget;

struct FASVFolderBadgeCache
{
    TMap<FName, int32>         FolderAssetCount;
    TMap<FName, EASVPriority>  FolderWorstSeverity;
    TArray<TWeakPtr<SWidget>>  LiveWidgets;

    void           Rebuild(const TArray<FASVValidationResult>& Results);
    int32          Get(FName Folder) const;
    EASVPriority   GetWorstSeverity(FName Folder) const;
    void           RegisterWidget(TSharedPtr<SWidget> Widget);
    void           InvalidateAllWidgets();

private:
    void PropagatePath(const FString& LeafFolder, EASVPriority Severity);
};
