// Copyright (c) 2026 Fergius Engineering.
#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserFrontEndFilterExtension.h"
#include "ASVContentBrowserFilterExtension.generated.h"

UCLASS()
class UASVContentBrowserFilterExtension : public UContentBrowserFrontEndFilterExtension
{
    GENERATED_BODY()
public:
    virtual void AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory, TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const override;
};
