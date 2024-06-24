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

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(FoundationEditor, All, All)

class FFoundationEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void AddPullDownMenu(FMenuBarBuilder& MenuBuilder);
	void FillMenu(FMenuBuilder& MenuBuilder);

	void OpenMetaplexConfiguration();
	TSharedRef<SDockTab> OnOpenMetaplexConfiguration(const FSpawnTabArgs& SpawnTabArgs);
	
private:
	TSharedPtr<class FUICommandList> PluginCommands;

public:
	static FFoundationEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FFoundationEditorModule>("FoundationEditor");
	}
};