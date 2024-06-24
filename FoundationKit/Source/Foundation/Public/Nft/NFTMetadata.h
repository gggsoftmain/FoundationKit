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

#include "SolanaUtils/SolanaKey.h"
#include "NFTMetadata.generated.h"

USTRUCT(BlueprintType)
struct FOUNDATION_API FNFTCreator
{
	GENERATED_BODY();

public:
	FNFTCreator() {}
	FNFTCreator(FPublicKey key, uint8 share, bool verified);

	FPublicKey PublicKey;
	uint8 Share;
	bool Verified;
	
	TArray<uint8> Encode();
	TArray<uint8> Encode() const;
};


USTRUCT(BlueprintType)
struct FOUNDATION_API FNFTMetadata
{
	GENERATED_BODY()

	FString Name;

	FString Symbol;

	FString Uri;

	int SellerFeeBasisPoints;

	TArray<FNFTCreator> Creators;
};