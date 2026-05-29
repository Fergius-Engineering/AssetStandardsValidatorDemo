// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTrigger.h"
#include "ASVTrigger_OnStartup.generated.h"

/**
 * Trigger that fires once after the editor finishes initialization.
 * Collects all assets from scan roots and triggers validation.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "On Editor Startup")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_OnStartup : public UASVTrigger
{
	GENERATED_BODY()

public:
	/** Delay after activation before running (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (ClampMin = "0.0"))
	float DelaySeconds = 0.5f;

#ifdef ASV_DEMO
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	virtual FText GetDisplayName_Implementation() const override;

private:
	bool RunDeferredAudit(float DeltaTime);
};
