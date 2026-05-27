// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

/** General plugin lifecycle: subsystem init, discovery, config */
DECLARE_LOG_CATEGORY_EXTERN(LogASV, Log, All);

/** Validator execution: rule matching, prefix resolution, folder checks */
DECLARE_LOG_CATEGORY_EXTERN(LogASVValidation, Log, All);

/** Trigger lifecycle: activation, deactivation, firing */
DECLARE_LOG_CATEGORY_EXTERN(LogASVTrigger, Log, All);

/** Auto-fix operations: rename, move, results */
DECLARE_LOG_CATEGORY_EXTERN(LogASVAutoFix, Log, All);

/** Commandlet: CI execution, report generation */
DECLARE_LOG_CATEGORY_EXTERN(LogASVCommandlet, Log, All);

// Structured log macros — automatically embed function name and optional context
// (asset path, rule ID, checker name) so every line is grep-friendly.
//
// Usage:
//   ASV_LOG(LogASV, Display, TEXT("Discovered %d checkers"), Num);
//   ASV_ALOG(LogASVValidation, Verbose, AssetPath, TEXT("Prefix resolved to %s"), *Prefix);
//   ASV_RLOG(LogASVValidation, Verbose, AssetPath, RuleID, TEXT("Rule matched"));
//   ASV_CLOG(LogASV, Log, CheckerName, TEXT("Running %d validators"), Num);

/** Base: [FunctionName] message */
#define ASV_LOG(Category, Verbosity, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs] ") Fmt, __FUNCTION__, ##__VA_ARGS__)

/** Asset context: [FunctionName][AssetPath] message */
#define ASV_ALOG(Category, Verbosity, AssetPath, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s] ") Fmt, __FUNCTION__, *(AssetPath), ##__VA_ARGS__)

/** Rule context: [FunctionName][AssetPath][RuleID] message */
#define ASV_RLOG(Category, Verbosity, AssetPath, RuleID, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s][%s] ") Fmt, __FUNCTION__, *(AssetPath), *FString(RuleID), ##__VA_ARGS__)

/** Checker context: [FunctionName][CheckerName] message */
#define ASV_CLOG(Category, Verbosity, CheckerName, Fmt, ...) \
	UE_LOG(Category, Verbosity, TEXT("[%hs][%s] ") Fmt, __FUNCTION__, *(CheckerName), ##__VA_ARGS__)

/** Apply verbose logging to all ASV categories (called from GlobalSettings) */
void ASV_SetVerboseLogging(bool bEnable);
