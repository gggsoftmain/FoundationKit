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

#include "FoundationEditorCommands.h"
#include "FoundationEditorStyle.h"

#define LOCTEXT_NAMESPACE "FoundationEditor"

FFoundationEditorCommands::FFoundationEditorCommands()
	: TCommands<FFoundationEditorCommands>(TEXT("Solana"), NSLOCTEXT("Contexts", "Solana", "Solana Plugin"), NAME_None, FFoundationEditorStyle::GetStyleSetName())
{

}

void FFoundationEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenSolanaCombobox, "Solana", "Solana", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenMetaplexConfiguration, "Open Metaplex Configuration", "Open Metaplex Configuration", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
