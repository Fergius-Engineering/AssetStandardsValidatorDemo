// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTrigger.h"
#include "ASVTrigger_OnAssetRenamed.generated.h"

/**
 * Trigger that fires when an asset is renamed in the Content Browser.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "On Asset Renamed")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_OnAssetRenamed : public UASVTrigger
{
	GENERATED_BODY()

public:
	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	virtual FText GetDisplayName_Implementation() const override;

private:
	FDelegateHandle RenamedHandle;
};
