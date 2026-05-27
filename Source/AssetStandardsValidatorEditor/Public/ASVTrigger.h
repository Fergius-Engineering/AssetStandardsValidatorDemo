// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AssetRegistry/AssetData.h"
#include "ASVTrigger.generated.h"

class UASVCheckerConfig;

/** Delegate fired when a trigger activates with relevant assets */
DECLARE_MULTICAST_DELEGATE_OneParam(FASVOnTriggerFired, const TArray<FAssetData>&);

/**
 * Base class for all ASV triggers.
 * Listens for engine events (save, startup, PIE etc.) and fires when assets need validation.
 * Instances live inside UASVCheckerConfig DataAssets as subobjects.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories)
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger : public UObject
{
	GENERATED_BODY()

public:
	/** Whether this trigger is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	bool bEnabled = true;

	/** C++ delegate — the subsystem binds to this */
	FASVOnTriggerFired OnTriggerFired;

	/** Start listening for events. Override to bind to engine delegates. */
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
	void Activate();
	virtual void Activate_Implementation() {}

	/** Stop listening. Override to unbind engine delegates. */
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
	void Deactivate();
	virtual void Deactivate_Implementation() {}

	/** Get the owning checker config */
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	UASVCheckerConfig* GetOwningChecker() const;

	/** Get a display name for this trigger */
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
	FText GetDisplayName() const;
	virtual FText GetDisplayName_Implementation() const;

protected:
	/** Call this from subclasses when the trigger condition is met */
	void BroadcastTriggered(const TArray<FAssetData>& Assets);
};
