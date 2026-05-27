// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTrigger.h"
#include "ASVTrigger_Manual.generated.h"

/**
 * Manual trigger — does not listen to any events automatically.
 * Must be fired explicitly via FireManually(), Python scripts, toolbar button, or commandlet.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Manual")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_Manual : public UASVTrigger
{
	GENERATED_BODY()

public:
	/** Manually fire this trigger with the given assets */
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void FireManually(const TArray<FAssetData>& Assets);

	virtual FText GetDisplayName_Implementation() const override;
};
