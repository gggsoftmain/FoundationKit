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
#include "SolanaUtils/Utils/Types.h"
#include "SolanaKey.generated.h"

struct FPrivateKey;

USTRUCT(BlueprintType)
struct FOUNDATION_API FSolanaKey
{
	GENERATED_BODY()

public:
	FSolanaKey() { IsValid = false; }
	virtual ~FSolanaKey() {}
	virtual int GetKeyLength() { check(0 && "You must override this"); return 0; }

	void SetKey(FString key);
	FString GetKey() { return Key; }
	const FString GetKey() const { return Key; }

	void SetKeyData(TArray<uint8>& keyData);
	void SetKeyData(const TArray<uint8>& keyData);
	TArray<uint8>& GetKeyData() { return KeyData; }
	const TArray<uint8>& GetKeyData() const { return KeyData; }

	bool GetIsValid() { return IsValid; }
	bool GetIsValid() const { return IsValid; }
public:
	static bool GenerateKeyPair(const TArray<uint8>& Seed, FPublicKey& publicKey, FPrivateKey& privateKey);

	bool operator==(const FSolanaKey& Other) const
	{
		return Key == Other.Key;
	}

	bool operator==(const FString& Other) const
	{
		return Key == Other;
	}

	bool operator==(const TArray<uint8>& Other) const
	{
		return KeyData == Other;
	}

protected:
	UPROPERTY(SaveGame, BlueprintReadOnly)
	FString Key;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	TArray<uint8> KeyData;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	bool IsValid;
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FPublicKey : public FSolanaKey
{
	GENERATED_BODY()
public:
	FPublicKey();
	FPublicKey(FString key);
	virtual ~FPublicKey();

	FORCEINLINE int GetKeyLength() override { return PublicKeySize; }
	static FORCEINLINE int GetKeyLengthStatic() { return PublicKeySize; }

	void SetKeyFromPrivate(const FPrivateKey& privatekey);

	bool IsOnCurve() const;

	static FPublicKey FindMetadataPDA(const FPublicKey& mintAddress);
	static FPublicKey FindMasterEditionPDA(const FPublicKey& mintAddress);
	static bool FindProgramAddress(const TArray<TArray<uint8>>& seeds, const FPublicKey& programId, FPublicKey& outPublicKey);

private:
	static bool CreateProgramAddress(const TArray<TArray<uint8>>& seeds, const FPublicKey& programId, FPublicKey& outPublicKey);
};

USTRUCT(BlueprintType)
struct FOUNDATION_API FPrivateKey : public FSolanaKey
{
	GENERATED_BODY()
public:
	FPrivateKey();
	virtual ~FPrivateKey();

	FORCEINLINE int GetKeyLength() override { return PrivateKeySize; }
	static FORCEINLINE int GetKeyLengthStatic() { return PrivateKeySize; }
};