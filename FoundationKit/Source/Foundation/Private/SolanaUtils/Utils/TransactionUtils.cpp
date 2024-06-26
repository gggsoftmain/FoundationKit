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
*/

#include "TransactionUtils.h"

#include "Crypto/Base58.h"
#include "Crypto/FEd25519Bip39.h"
#include "SolanaUtils/Instructions.h"
#include "SolanaUtils/Mnemonic.h"
#include "SolanaUtils/Transaction.h"
#include "SolanaUtils/Account.h"
#include "SolanaUtils/Program/CommonPrograms.h"

TArray<uint8> FTransactionUtils::TransferSOLTransaction(const FAccount& from, const FAccount& to, int64 amount, const FString& blockHash)
{
	FTransaction transaction(blockHash);
	transaction.AddInstruction(FInstruction::TransferLamports(from, to, amount));
	return transaction.Build(from);
}

TArray<uint8> FTransactionUtils::TransferTokenTransaction(const FAccount& from, const FAccount& to, const FAccount& owner, int64 amount, const FString& mint, const FString& blockHash, const FString& existingAccount)
{
	TArray<FAccount> signers;
	
	FTransaction transaction(blockHash);
	if(existingAccount.IsEmpty()) 
	{
		const FMnemonic mnemonic(24);
		FEd25519Bip39 keypair(mnemonic.DeriveSeed());
		const FAccount newKeypair = FAccount::FromSeed(keypair.DeriveAccountPath(0));
		const int newAccountSize = 2039280;

		signers.Add(owner);
		signers.Add(newKeypair);
		
		transaction.AddInstruction(FInstruction::CreateAccount(owner, newKeypair, newAccountSize));
		transaction.AddInstruction(FInstruction::InitializeTokenAccount(newKeypair, FPublicKey(mint), to));
		transaction.AddInstruction(FInstruction::TransferTokens(from, newKeypair, owner, amount));
	}
	else
	{
		signers.Add(owner);
		transaction.AddInstruction(FInstruction::TransferTokens(from, FAccount::FromPublicKey(existingAccount), owner, amount));
	}
	
	return transaction.Build(signers);
}

TArray<uint8> FTransactionUtils::MintingTransaction(const FAccount& owner, const FAccount& mint, const FPublicKey& associatedTokenAccount,
	int64 minimumRent, const FString& blockHash, const FNFTMetadata& metadata)
{
	TArray<FAccount> signers;
	
	signers.Add(owner);
	signers.Add(mint);

	FTransaction transaction(blockHash);
	transaction.SetFeePayer(owner.PublicKey);

	transaction.AddInstruction(FInstruction::CreateSystemAccount(owner.PublicKey, mint.PublicKey, minimumRent, MintAccountDataSize, FTokenProgram::ProgramIdKey));
	transaction.AddInstruction(FInstruction::TokenProgramInitializeMint(mint.PublicKey, 0, owner.PublicKey, &owner.PublicKey));
	transaction.AddInstruction(FInstruction::CreateAssociatedTokenAccount(owner.PublicKey, owner.PublicKey, mint.PublicKey));
	transaction.AddInstruction(FInstruction::TokenProgramMintTo(mint.PublicKey, associatedTokenAccount, 1, owner.PublicKey));
	transaction.AddInstruction(FInstruction::CreateMetadataAccount(FPublicKey::FindMetadataPDA(mint.PublicKey), mint.PublicKey, owner.PublicKey, owner.PublicKey, owner.PublicKey, metadata, true, true, 0));
	transaction.AddInstruction(FInstruction::CreateMasterEdition(FPublicKey::FindMasterEditionPDA(mint.PublicKey), mint.PublicKey, owner.PublicKey, owner.PublicKey, owner.PublicKey, FPublicKey::FindMetadataPDA(mint.PublicKey)));

	return transaction.Build(signers);
}