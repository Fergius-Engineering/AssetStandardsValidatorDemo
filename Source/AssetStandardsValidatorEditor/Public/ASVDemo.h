// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

#ifdef ASV_DEMO

class FASVDemoSession
{
public:
    static constexpr int32 AssetLimit = 200;
    static constexpr int32 FixSessionLimit = 5;

    static FString GetFabUrl();

    // unified fix counter — counts every auto-fix (single or batch) per editor session
    static bool CanFix();
    static void RecordFix();
    static int32 GetFixCount() { return FixCount; }

    // shows modal notice once per session before any manual scan
    static void ShowScanNoticeOnce();
    // shows modal notice every time — for panel button
    static void ShowScanNotice();

private:
    static int32 FixCount;
    static bool bScanNoticeShown;
};

#endif // ASV_DEMO
