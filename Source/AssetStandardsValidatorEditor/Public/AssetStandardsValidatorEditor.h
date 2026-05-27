// Copyright (c) 2026 Fergius Engineering.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Editor module for the Asset Standards Validator plugin.
 * Handles startup, toolbar, and MessageLog registration.
 * All validation logic lives in UASVEditorSubsystem, validators, and triggers.
 */
class FAssetStandardsValidatorEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static const FName ASVPanelTabId;
	static void RunAnalysisFromToolbar();

private:
	static void RegisterMenus();
	static void AddToolbarButton(class FToolBarBuilder& Builder);
	static TSharedRef<class SDockTab> SpawnTab(const class FSpawnTabArgs& Args);

	TSharedPtr<class FUICommandList> PluginCommands;
	IConsoleCommand* ExportConsoleCommand = nullptr;
	bool bCBExtensionsRegistered = false;
};
