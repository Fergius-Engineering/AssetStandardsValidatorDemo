// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SToolTip.h"
#include "ASVTypes.h"

// Hover tooltip for validation results.
// Shows Path/Class info row + severity-colored error card + dev-details (after 4s).
// No thumbnail — designed for list-view rows where the asset is already identified by name.
namespace ASVAssetHoverTooltip
{
    // single result — used in the ASV panel list rows (includes dev-details after 3s)
    TSharedRef<SToolTip> Build(const FASVValidationResult& Result);

    // multiple results — used for Content Browser overlay (no dev-details section)
    TSharedRef<SToolTip> BuildMulti(const TArray<FASVValidationResult>& Results);
}
