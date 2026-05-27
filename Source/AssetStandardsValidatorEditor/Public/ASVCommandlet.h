// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "ASVTypes.h"
#include "ASVCommandlet.generated.h"

/**
 * Commandlet for running ASV checks from the command line (CI/pre-commit).
 *
 * Usage:
 *   UnrealEditor-Cmd.exe Project.uproject -run=ASV [-checkers=Name1,Name2] [-all] [-severity=P0]
 *       [-format=csv|json|html -output=C:/Reports/ [-milestone=Alpha]]
 *       [-report=report.json]  (legacy: writes json to a specific file path)
 *
 * Exit codes:
 *   0 = no violations at the specified severity
 *   1 = violations found
 *   2 = error (bad output path, unknown format, etc.)
 */
UCLASS()
class UASVCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UASVCommandlet();
	virtual int32 Main(const FString& Params) override;

	/** Parse severity string (P0/P1/P2/P3) into enum. Defaults to P0 for invalid input. */
	static EASVPriority ParseSeverity(const FString& SeverityStr);

	/** Determine if the commandlet should return failure for the given report and minimum severity. */
	static bool ShouldFailForSeverity(const FASVAuditReport& Report, EASVPriority MinSeverity);

	/** Serialize audit report to JSON string. */
	static FString SerializeReportToJson(const FASVAuditReport& Report);
};
