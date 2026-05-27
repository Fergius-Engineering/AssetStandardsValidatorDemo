// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTrigger.h"
#include "ASVTrigger_OnAssetCreated.generated.h"

/**
 * Trigger that fires when a new asset is created in the Content Browser.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "On Asset Created")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_OnAssetCreated : public UASVTrigger
{
	GENERATED_BODY()

public:
	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	virtual FText GetDisplayName_Implementation() const override;

private:
	FDelegateHandle CreatedHandle;
};
