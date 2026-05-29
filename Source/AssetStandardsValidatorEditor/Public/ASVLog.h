// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogASV, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogASVValidation, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogASVTrigger, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogASVAutoFix, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogASVCommandlet, Log, All);

// structured log macros — embed function name + optional context
#define ASV_LOG(Category, Verbosity, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs] ") Fmt, __FUNCTION__, ##__VA_ARGS__)

#define ASV_ALOG(Category, Verbosity, AssetPath, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s] ") Fmt, __FUNCTION__, *(AssetPath), ##__VA_ARGS__)

#define ASV_RLOG(Category, Verbosity, AssetPath, RuleID, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s][%s] ") Fmt, __FUNCTION__, *(AssetPath), *FString(RuleID), ##__VA_ARGS__)

#define ASV_CLOG(Category, Verbosity, CheckerName, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s] ") Fmt, __FUNCTION__, *(CheckerName), ##__VA_ARGS__)

void ASV_SetVerboseLogging(bool bEnable);
