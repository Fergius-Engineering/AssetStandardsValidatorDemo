// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"

class FASVFeedback
{
public:
    // feedback popup
    static bool ShouldShow();
    static void MarkShown();
    static FString GetFormUrl();
    static void OpenUrl(); // opens browser directly, no dialog

    // first-run flag (opens index.html on first editor launch in demo)
    static bool HasSeenWelcome();
    static void MarkWelcomeSeen();

private:
    static constexpr const TCHAR* IniSection     = TEXT("ASVFeedback");
    static constexpr const TCHAR* IniKeyFeedback = TEXT("FeedbackShown");
    static constexpr const TCHAR* IniKeyWelcome  = TEXT("WelcomeSeen");
};
