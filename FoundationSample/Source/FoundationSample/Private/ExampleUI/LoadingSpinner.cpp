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

#include "ExampleUI/LoadingSpinner.h"

UUserWidget* ULoadingSpinner::LoadingSpinner;

void ULoadingSpinner::StartLoading(UWidget* owner)
{
    FSoftClassPath LoadingSpinnerClass(TEXT("/FoundationSample/UI/Wallet/WBP_LoadingSpinner.WBP_LoadingSpinner_C"));
    UClass* WidgetBPClass = LoadingSpinnerClass.TryLoadClass<UUserWidget>();
    if (WidgetBPClass == nullptr)
    {
        return;
    }

	if (LoadingSpinner != nullptr)
	{
		return;
	}

    LoadingSpinner = CreateWidget<UUserWidget>(owner, WidgetBPClass);

    // À§Á¬À» ºäÆ÷Æ®¿¡ Ãß°¡
    if (LoadingSpinner != nullptr)
    {
        LoadingSpinner->AddToViewport();
    }
}

void ULoadingSpinner::EndLoading()
{
    if (LoadingSpinner != nullptr)
    {
        LoadingSpinner->RemoveFromParent();
        LoadingSpinner = nullptr;
    }
}