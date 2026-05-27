// Copyright (c) 2026 Fergius Engineering.
#pragma once

#include "CoreMinimal.h"
#include "FrontendFilterBase.h"
#include "ContentBrowserItem.h"
#include "ASVCompat.h"
#include "ASVTypes.h"

class UASVEditorSubsystem;

// returns the IssuesByAsset key for a filter item
inline FName ASVGetFilterPath(const FContentBrowserItem& Item)
{
    FAssetData D;
    return Item.Legacy_TryGetAssetData(D) ? ASV_GetAssetPathName(D) : NAME_None;
}

class FASVFrontendFilter_HasError : public FFrontendFilter
{
public:
    FASVFrontendFilter_HasError(TSharedPtr<FFrontendFilterCategory> InCategory, UASVEditorSubsystem* InSub);

    virtual FString GetName() const override;
    virtual FText GetDisplayName() const override;
    virtual FText GetToolTipText() const override;
    virtual FLinearColor GetColor() const override;
    virtual bool PassesFilter(FContentBrowserItem const& InItem) const override;
    void RefreshFilter();

private:
    TWeakObjectPtr<UASVEditorSubsystem> Subsystem;
};

class FASVFrontendFilter_Severity : public FFrontendFilter
{
public:
    FASVFrontendFilter_Severity(TSharedPtr<FFrontendFilterCategory> InCategory, UASVEditorSubsystem* InSub, EASVPriority InSeverity);

    virtual FString GetName() const override;
    virtual FText GetDisplayName() const override;
    virtual FText GetToolTipText() const override;
    virtual FLinearColor GetColor() const override;
    virtual bool PassesFilter(FContentBrowserItem const& InItem) const override;
    void RefreshFilter();

private:
    TWeakObjectPtr<UASVEditorSubsystem> Subsystem;
    EASVPriority FilterSeverity;
};

class FASVFrontendFilter_Rule : public FFrontendFilter
{
public:
    FASVFrontendFilter_Rule(TSharedPtr<FFrontendFilterCategory> InCategory, UASVEditorSubsystem* InSub, FName InRuleID);

    virtual FString GetName() const override;
    virtual FText GetDisplayName() const override;
    virtual FText GetToolTipText() const override;
    virtual bool PassesFilter(FContentBrowserItem const& InItem) const override;
    void RefreshFilter();

    FName GetRuleID() const { return RuleID; }
    void SetDominantSeverity(EASVPriority Sev) { DominantSeverity = Sev; }

private:
    TWeakObjectPtr<UASVEditorSubsystem> Subsystem;
    FName RuleID;
    EASVPriority DominantSeverity = EASVPriority::P3;
    bool bSeverityKnown = false;
};
