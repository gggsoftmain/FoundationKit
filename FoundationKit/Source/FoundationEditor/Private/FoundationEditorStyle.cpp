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

#include "FoundationEditorStyle.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FFoundationEditorStyle::StyleInstance = NULL;

void FFoundationEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FFoundationEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FFoundationEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("FoundationEditorStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FFoundationEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("FoundationEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Foundation")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Foundation.OpenSolanaCombobox", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon128"), TEXT(".png")), Icon40x40));
	Style->Set("Foundation.OpenSolanaCombobox.Small", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon128"), TEXT(".png")), Icon20x20));

	return Style;
}

void FFoundationEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FFoundationEditorStyle::Get()
{
	return *StyleInstance;
}
