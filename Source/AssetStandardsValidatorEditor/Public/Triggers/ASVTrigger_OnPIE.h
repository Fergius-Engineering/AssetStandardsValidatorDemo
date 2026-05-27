// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTrigger.h"
#include "ASVTrigger_OnPIE.generated.h"

/**
 * Trigger that fires when Play In Editor (PIE) starts.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "On PIE Start")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_OnPIE : public UASVTrigger
{
	GENERATED_BODY()

public:
	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	virtual FText GetDisplayName_Implementation() const override;

private:
	FDelegateHandle PIEHandle;
};
