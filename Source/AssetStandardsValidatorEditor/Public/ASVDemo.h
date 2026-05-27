// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#ifdef ASV_DEMO

class FASVDemoSession
{
public:
    static constexpr int32 AssetLimit = 200;
    static constexpr int32 BatchFixSessionLimit = 5;

    static FString GetFabUrl();

    // batch fix session counter (in-memory, resets on editor restart)
    static bool CanBatchFix();
    static void RecordBatchFix();
    static int32 GetBatchFixCount() { return BatchFixCount; }

private:
    static int32 BatchFixCount;
};

#endif // ASV_DEMO
