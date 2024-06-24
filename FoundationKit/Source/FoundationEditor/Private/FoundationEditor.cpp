/*
Copyright 2024 GGGSoft.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include "FoundationEditor.h"
#include "Foundation.h"
#include "FoundationEditorCommands.h"
#include "FoundationEditorStyle.h"

#include "SMetaplexConfigurationWidget.h"

#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FFoundationEditorModule, FoundationEditor);

DEFINE_LOG_CATEGORY(FoundationEditor)

static const FName SMetaplexConfigurationTabName("Metaplex Configuration");

#define LOCTEXT_NAMESPACE "FoundationEditor"

FFoundationEditorModule* g_FoundationEditorModule = nullptr;


void FFoundationEditorModule::StartupModule()
{
	g_FoundationEditorModule = this;

	FFoundationEditorStyle::Initialize();
	FFoundationEditorStyle::ReloadTextures();

	FFoundationEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FFoundationEditorCommands::Get().OpenMetaplexConfiguration,
		FExecuteAction::CreateRaw(this, &FFoundationEditorModule::OpenMetaplexConfiguration),
		FCanExecuteAction());

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SMetaplexConfigurationTabName, FOnSpawnTab::CreateRaw(this, &FFoundationEditorModule::OnOpenMetaplexConfiguration))
		.SetDisplayName(LOCTEXT("FoundationWidget", "Metaplex Configuration"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Add custom commands to main menu bar
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> newExtender = MakeShareable(new FExtender);
	newExtender->AddMenuBarExtension(
		"Help",
		EExtensionHook::After,
		PluginCommands,
		FMenuBarExtensionDelegate::CreateRaw(this, &FFoundationEditorModule::AddPullDownMenu)
	);
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(newExtender);
}

void FFoundationEditorModule::ShutdownModule()
{
	g_FoundationEditorModule = nullptr;
	
	FFoundationEditorStyle::Shutdown();

	FFoundationEditorCommands::Unregister();
}

void FFoundationEditorModule::AddPullDownMenu(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		FText::FromString("Solana"),
		FText::FromString("Solana"),
		FNewMenuDelegate::CreateRaw(this, &FFoundationEditorModule::FillMenu),
		"Custom"
	);
}

void FFoundationEditorModule::FillMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("Metaplex", LOCTEXT("Metaplex", "Metaplex"));
	{
		MenuBuilder.AddMenuEntry(FFoundationEditorCommands::Get().OpenMetaplexConfiguration, NAME_None, FText::FromName(SMetaplexConfigurationTabName));
	}
	MenuBuilder.EndSection();
}

void FFoundationEditorModule::OpenMetaplexConfiguration()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SMetaplexConfigurationTabName);
}

TSharedRef<SDockTab> FFoundationEditorModule::OnOpenMetaplexConfiguration(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				[
					SNew(SMetaplexConfigurationWidget)
				]
		];
}

#undef LOCTEXT_NAMESPACE
