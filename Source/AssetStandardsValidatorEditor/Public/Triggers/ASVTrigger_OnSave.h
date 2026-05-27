// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "UObject/ObjectSaveContext.h"

#include "ASVTrigger.h"
#include "CoreMinimal.h"

#include "ASVTrigger_OnSave.generated.h"

/**
 * Trigger that fires when assets are saved.
 * Debounces multiple rapid saves into a single validation pass.
 * Uses direct UObject reference to ensure fresh data (not stale AssetRegistry).
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "On Save")
class ASSETSTANDARDSVALIDATOREDITOR_API UASVTrigger_OnSave : public UASVTrigger
{
    GENERATED_BODY()

public:
    /** Delay before flushing accumulated saves (seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (ClampMin = "0.05"))
    float DebounceSeconds = 0.25f;

    virtual void Activate_Implementation() override;
    virtual void Deactivate_Implementation() override;
    virtual FText GetDisplayName_Implementation() const override;

private:
    void OnObjectPreSaved(UObject* Object, FObjectPreSaveContext SaveContext);
    bool FlushPendingSaved(float DeltaTime);

    TArray<TWeakObjectPtr<UObject>> PendingSavedObjects;
    bool bFlushScheduled = false;
    FDelegateHandle SaveDelegateHandle;
};
