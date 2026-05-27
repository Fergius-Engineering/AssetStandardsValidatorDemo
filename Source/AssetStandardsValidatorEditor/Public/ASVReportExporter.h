// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "ASVTypes.h"

struct ASSETSTANDARDSVALIDATOREDITOR_API FASVExportOptions
{
    FString MilestoneLabel;
    // checker names for the case where violations = 0 (no CheckerName in results)
    TArray<FString> ActiveCheckerNames;
    // "fresh_audit" | "accumulated" | "filtered"
    FString AuditSource = TEXT("fresh_audit");
};

struct ASSETSTANDARDSVALIDATOREDITOR_API FASVDiffResult
{
    bool bValid = false;
    FString PreviousTimestamp;
    int32 DeltaTotal = 0;
    int32 DeltaP0 = 0, DeltaP1 = 0, DeltaP2 = 0, DeltaP3 = 0;
    int32 NewCount = 0, ResolvedCount = 0;
    TArray<FASVValidationResult> NewViolations;
};

class ASSETSTANDARDSVALIDATOREDITOR_API FASVReportExporter
{
public:
    // dispatch by format string: "csv", "json", "html"
    // writes timestamped file + ASV_Report_latest.<ext> + .asv-report.json companion
    static bool Export(const FASVAuditReport& Report, const FString& Format, const FString& OutputDir, const FASVExportOptions& Options = {});

    static bool ExportCSV(const FASVAuditReport& Report, const FString& FilePath, const FASVExportOptions& Options = {});
    static bool ExportJSON(const FASVAuditReport& Report, const FString& FilePath, const FASVExportOptions& Options = {});
    static bool ExportHTML(const FASVAuditReport& Report, const FString& FilePath, const FASVExportOptions& Options = {});

private:
    static void WriteCompanion(const FASVAuditReport& Report, const FString& OutputDir, const FASVExportOptions& Options);
    static FASVDiffResult ComputeDiff(const FASVAuditReport& Current, const FString& CompanionPath, const FASVExportOptions& Options = {});

    static FString BuildCSVBlock(const FASVAuditReport& Report, const FASVExportOptions& Options);
    static FString BuildJSONString(const FASVAuditReport& Report, const FASVExportOptions& Options, const FASVDiffResult& Diff);
    static FString BuildHTMLString(const FASVAuditReport& Report, const FASVExportOptions& Options, const FASVDiffResult& Diff);

    static FString EscapeCSV(const FString& Value);
    static FString EscapeHTML(const FString& Value);
    static FString SeverityToString(EASVPriority Sev);
    static FString GetProjectName();
    static FString GetASVVersion();
    static FString GetTimestamp();
    static TMap<FString, FString> ReadCustomMetadata();

    static int32 CountAssetsWithViolations(const FASVAuditReport& Report);
    static int32 CountAssetsWithCriticalViolations(const FASVAuditReport& Report);
    static float ComputeHealthScore(const FASVAuditReport& Report);
    static TArray<FString> DeriveCheckerNames(const FASVAuditReport& Report, const FASVExportOptions& Options);
};
