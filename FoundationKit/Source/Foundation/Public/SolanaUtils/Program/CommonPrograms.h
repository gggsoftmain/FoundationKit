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
#include "SolanaUtils/SolanaKey.h"
#include "CommonPrograms.generated.h"

USTRUCT(BlueprintType)
struct FOUNDATION_API FSystemProgram
{
	GENERATED_BODY()

public:
	const static FName ProgramIdName;
	const static FPublicKey ProgramIdKey;
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FTokenProgram
{
	GENERATED_BODY()

public:
	const static FName ProgramIdName;
	const static FPublicKey ProgramIdKey;
};


USTRUCT(BlueprintType)
struct FOUNDATION_API FATAProgram
{
	GENERATED_BODY()

public:
	static FPublicKey GetAssociatedTokenAddress(
		const FPublicKey& owner, 
		const FPublicKey& mint,
		bool allowOwnerOffCurve = false,
		const FPublicKey& programId = FTokenProgram::ProgramIdKey,
		const FPublicKey& associatedTokenProgramId = FATAProgram::ProgramIdKey);

public:
	const static FName ProgramIdName;
	const static FPublicKey ProgramIdKey;
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FMetadataProgram
{
	GENERATED_BODY()

public:
	const static FName ProgramIdName;
	const static FPublicKey ProgramIdKey;
};
