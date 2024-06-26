﻿/*
Copyright 2022 ATMTA, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Jon Sawler
Contributers: Daniele Calanna, Riccardo Torrisi, Federico Arona
*/
#pragma once

#include "Types.generated.h"

// |  ENT  | CS | ENT+CS |  MS  |
// +-------+----+--------+------+
// |  128  |  4 |   132  |  12  |
// |  160  |  5 |   165  |  15  |
// |  192  |  6 |   198  |  18  |
// |  224  |  7 |   231  |  21  |
// |  256  |  8 |   264  |  24  |

constexpr int PublicKeySize = 32;
constexpr int PrivateKeySize = 64;

constexpr int Base58PubKeySize = 44;
constexpr int Base58PrKeySize = 88;

constexpr int MintAccountDataSize = 82;
constexpr int AccountDataSize = 165;

UENUM()
enum class ESystemInstruction : uint32
{
	CreateAccount = 0,
	Assign = 1,
	Transfer = 2,
	CreateAccountWithSeed = 3,
	AdvanceNonceAccount = 4,
	WithdrawNonceAccount = 5,
	InitializeNonceAccount = 6,
	AuthorizeNonceAccount = 7,
	Allocate = 8,
	AllocateWithSeed = 9,
	AssignWithSeed = 10,
	TransferWithSeed = 11,
	UpgradeNonceAccount = 12
};

UENUM()
enum class ETokenInstruction : uint32
{
	InitializeMint = 0,
	InitializeAccount = 1,
	InitializeMultisig = 2,
	Transfer = 3,
	Approve = 4,
	Revoke = 5,
	SetAuthority = 6,
	MintTo = 7,
	Burn = 8,
	CloseAccount = 9,
	FreezeAccount = 10,
	ThawAccount = 11,
	TransferChecked = 12,
	ApproveChecked = 13,
	MintToChecked = 14,
	BurnChecked = 15,
	InitializeAccount2 = 16,
	SyncNative = 17,
	InitializeAccount3 = 18,
	InitializeMultisig2 = 19,
	InitializeMint2 = 20,
	GetAccountDataSize = 21,
	InitializeImmutableOwner = 22,
	AmountToUiAmount = 23,
	UiAmountToAmount = 24,
	InitializeMintCloseAuthority = 25,
	TransferFeeExtension = 26,
	ConfidentialTransferExtension = 27,
	DefaultAccountStateExtension = 28,
	Reallocate = 29,
	MemoTransferExtension = 30,
	CreateNativeMint = 31,
	InitializeNonTransferableMint = 32,
	InterestBearingMintExtension = 33,
	CpiGuardExtension = 34,
	InitializePermanentDelegate = 35,
	TransferHookExtension = 36,
	// ConfidentialTransferFeeExtension = 37,
	// WithdrawalExcessLamports = 38,
	MetadataPointerExtension = 39,
	GroupPointerExtension = 40,
	GroupMemberPointerExtension = 41,
};

USTRUCT()
struct FBalanceContextJson
{
	GENERATED_BODY()
	UPROPERTY()	double slot;
};

USTRUCT()
struct FBalanceResultJson
{
	GENERATED_BODY()
	UPROPERTY()	FBalanceContextJson context;
	UPROPERTY()	double value;
};

USTRUCT()
struct FAccountInfoJson
{
	GENERATED_BODY()

	UPROPERTY()	TArray<FString> data;
	UPROPERTY()	bool executable;
	UPROPERTY()	double lamports;
	UPROPERTY()	FString owner;
	UPROPERTY()	int32 rentEpoch;
};

USTRUCT()
struct FProgramAccountJson
{
	GENERATED_BODY()

	UPROPERTY()	FString data;
	UPROPERTY()	bool executable;
	UPROPERTY()	double lamports;
	UPROPERTY()	FString owner;
	UPROPERTY()	double rentEpoch;
};


USTRUCT()
struct FTokenUIBalanceJson
{
	GENERATED_BODY()

	UPROPERTY()	FString amount;
	UPROPERTY()	double decimals;
	UPROPERTY()	double uiAmount;
	UPROPERTY()	FString uiAmountString;
};

USTRUCT()
struct FTokenInfoJson
{
	GENERATED_BODY()

	UPROPERTY()	FTokenUIBalanceJson tokenAmount;
	UPROPERTY()	FString delegate;
	UPROPERTY()	FTokenUIBalanceJson delegatedAmount;
	UPROPERTY()	FString state;
	UPROPERTY()	bool isNative;
	UPROPERTY()	FString mint;
	UPROPERTY()	FString owner;
};

USTRUCT()
struct FParsedTokenDataJson
{
	GENERATED_BODY()

	UPROPERTY()	FString accountType;
	UPROPERTY()	FTokenInfoJson info;
	UPROPERTY()	double space;
};

USTRUCT()
struct FTokenDataJson
{
	GENERATED_BODY()

	UPROPERTY()	FString program;
	UPROPERTY()	FParsedTokenDataJson parsed;
	UPROPERTY()	double space;
};

USTRUCT()
struct FTokenAccountDataJson
{
	GENERATED_BODY()

	UPROPERTY()	FTokenDataJson data;
	UPROPERTY()	bool executable;
	UPROPERTY()	double lamports;
	UPROPERTY()	FString owner;
	UPROPERTY()	double rentEpoch;
};

USTRUCT()
struct FTokenBalanceDataJson
{
	GENERATED_BODY()

	UPROPERTY()	FTokenAccountDataJson account;
	UPROPERTY()	FString pubkey;
};

USTRUCT()
struct FTokenAccountArrayJson
{
	GENERATED_BODY()

	UPROPERTY()	TArray<FTokenBalanceDataJson> value;
};
