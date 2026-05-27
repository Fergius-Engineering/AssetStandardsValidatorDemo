// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

class ASVDocsTab
{
public:
    static const FName TabId;

    static void RegisterDocsTab();
    static void OpenDocsTab();

    // returns empty string if plugin content dir not found
    static FString BuildDocsUrl(const FString& RelativePath = TEXT(""));
};
