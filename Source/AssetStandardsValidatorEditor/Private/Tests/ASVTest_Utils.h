// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTypes.h"

namespace ASV_TestUtils
{
	/** Helper to find a validation result by RuleID in an array */
	static const FASVValidationResult* FindByRuleID(const TArray<FASVValidationResult>& Results, const FName& RuleID)
	{
		return Results.FindByPredicate([&](const FASVValidationResult& R) { return R.RuleID == RuleID; });
	}
}
