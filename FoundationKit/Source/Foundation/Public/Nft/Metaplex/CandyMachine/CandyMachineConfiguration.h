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
#include "CandyMachineConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FOUNDATION_API FCandyMachineHiddenSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	bool UseHiddenSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FString URI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FString Base64Hash;
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FCandyMachineCreator
{
	GENERATED_BODY();
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FCandyMachineGuards
{
	GENERATED_BODY();
};

USTRUCT(BlueprintType)
 struct FOUNDATION_API FCandyMachineConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	double Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	int number;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FString Symbol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	int SellerFeeBasisPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	bool IsSequential;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FCandyMachineHiddenSettings HiddenSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	bool IsMutable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FCandyMachineGuards Guards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	FString cacheFilePath;
};

UCLASS()
class FOUNDATION_API UCandyMachineConfigurationTable : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TableRow")
	TArray<FCandyMachineConfiguration> m_CandyMachineConfigurations;
};