/*
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
*/
#pragma once
#include "Crypto/Base58.h"
#include "SolanaUtils/SolanaKey.h"
#include "SolanaUtils/Utils/Types.h"
#include "Nft/NFTMetadata.h"

struct FAccount;

struct FAccountMeta
{
	FAccountMeta(const FPublicKey& publicKey, bool signer, bool writable)
		: PublicKey(publicKey), Signer(signer), Writable(writable)
	{
	}
	
	FPublicKey PublicKey;
	bool Signer;
	bool Writable;
};

struct FInstructionData
{
	FPublicKey ProgramId;
	TArray<FAccountMeta> Keys;
	TArray<uint8> Data;
};

class FInstruction
{
public:

	static FInstructionData TransferLamports(const FAccount& from, const FAccount& to, int64 lamports);
	static FInstructionData CreateAccount(const FAccount& from, const FAccount& newAccount, int64 rent);
	
	static FInstructionData InitializeTokenAccount(const FAccount& account, const FPublicKey& mint, const FAccount& owner);
	static FInstructionData TransferTokens(const FAccount& from, const FAccount& to, const FAccount& owner, int64 amount);

	static FInstructionData CreateSystemAccount(const FAccount& from, const FAccount& newAccount, int64 rent, int64 accountDataSize);
	static FInstructionData CreateSystemAccount(const FPublicKey& from, const FPublicKey& newAccount, int64 rent, int64 accountDataSize, const FPublicKey& programId);

	static FInstructionData TokenProgramInitializeMint(const FPublicKey& mint, int decimals, const FPublicKey& mintAuthority, const FPublicKey* freezeAuthority);
	static FInstructionData TokenProgramMintTo(const FPublicKey& mint, const FPublicKey& destination, int64 amount, const FPublicKey& mintAuthority);

	static FInstructionData CreateAssociatedTokenAccount(const FPublicKey& payer, const FPublicKey& owner, const FPublicKey& mint);

	static FInstructionData CreateMetadataAccount(const FPublicKey& metadataPDA, const FPublicKey& mint, const FPublicKey& authority, const FPublicKey& payer, const FPublicKey& updateAuthority, const FNFTMetadata& data, bool isMutable, bool updateAuthorityIsSigner, uint64 collectionDetails = 0);
	static FInstructionData CreateMasterEdition(const FPublicKey& masterEditionPAD, const FPublicKey& mintKey, const FPublicKey& payer,	const FPublicKey& updateAuthority, const FPublicKey& mintAuthority, const FPublicKey& metadataPDA);
};
