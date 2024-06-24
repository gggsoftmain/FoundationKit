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

#include "Transaction.h"

#include "SolanaUtils/Account.h"
#include "Instructions.h"
#include "Crypto/Base58.h"
#include "Crypto/CryptoUtils.h"

FTransaction::FTransaction(const FString& currentBlockHash)
{
	BlockHash = currentBlockHash;

	RequiredSignatures = 0;
	ReadOnlySignedAccounts = 0;
	ReadOnlyUnsignedAccounts = 0;
}

void FTransaction::SetFeePayer(const FPublicKey& feePayer)
{
	FeePayer = feePayer;
}

void FTransaction::AddInstruction(const FInstructionData& instruction)
{
	Instructions.Add(instruction);
	for( const FAccountMeta& data: instruction.Keys)
	{
		AddAccountData(data);
	}
	AddAccountData(FAccountMeta(instruction.ProgramId, false, false));
}

void FTransaction::AddInstructions(const TArray<FInstructionData>& instructions)
{
	Instructions.Append(instructions);
	for(const FInstructionData& instruction: instructions)
	{
		AddInstruction(instruction);
	}
}

int FTransaction::GetAccountIndex(const FString& key) const
{
	return AccountList.IndexOfByPredicate([key](const FAccountMeta& data)
	{
		return data.PublicKey.GetKey() == key;
	});
}

TArray<uint8> FTransaction::Build(const FAccount& signer)
{
	TArray<FAccount> signers;
	signers.Add(signer);
	return Build(signers);
}

TArray<uint8> FTransaction::Build(const TArray<FAccount>& signers)
{
	UpdateAccountList(signers);

	const TArray<uint8> message = BuildMessage();

	TArray<uint8> result = Sign(message, signers);
	result.Append(message);
	
	return result;
}

void FTransaction::AddAccountData(const FAccountMeta& accountMeta)
{
	int index = GetAccountIndex(accountMeta.PublicKey.GetKey());
	if (index == INDEX_NONE)
	{
		AccountList.Add(accountMeta);
	}
	else if (!AccountList[index].Signer && accountMeta.Signer)
	{
		AccountList[index].Signer = true;
		AccountList[index].Writable = AccountList[index].Writable || accountMeta.Writable;
	}
	else if (!AccountList[index].Writable && accountMeta.Writable)
	{
		AccountList[index].Writable = true;
	}
}

void FTransaction::UpdateAccountList(const TArray<FAccount>& signers)
{
	AccountList.Sort([](const FAccountMeta& A, const FAccountMeta& B) {
		if (A.Signer != B.Signer)
		{
			return A.Signer && !B.Signer;
		}
		if (A.Writable != B.Writable)
		{
			return A.Writable && !B.Writable;
		}
		// Otherwise, sort by pubkey, stringwise.
		return A.PublicKey.GetKey().Compare(B.PublicKey.GetKey(), ESearchCase::CaseSensitive) <= 0;
	});

	TArray<FAccountMeta> newList;
	if (FeePayer.GetIsValid())
	{
		int feePayerIndex = GetAccountIndex(FeePayer.GetKey());
		if (feePayerIndex == INDEX_NONE)
		{
			newList.Add(FAccountMeta(FeePayer, true, true));
		}
		else
		{
			AccountList.RemoveAt(feePayerIndex);
			newList.Add(FAccountMeta(FeePayer, true, true));
		}
	}

	newList.Append(AccountList);
	AccountList = newList;
	//AccountList.Sort([](const FAccountMeta& A, const FAccountMeta& B) { return A.Writable && !B.Writable; });

	//for (const FAccount& account : signers)
	//{
	//	int index = 0;
	//	while( index != INDEX_NONE )
	//	{
	//		index = AccountList.IndexOfByPredicate([account](const FAccountMeta& entry){ return account.PublicKey == entry.PublicKeyData; } );
	//		if( index != INDEX_NONE )
	//		{
	//			AccountList.RemoveAt(index);
	//		}
	//	}
	//}

	////Sort writables to the top of list before readding Signers at the very top
	//AccountList.Sort([](const FAccountMeta& A, const FAccountMeta& B) { return A.Writable && !B.Writable; });

	//for (int i = 0; i < signers.Num(); i++)
	//{
	//	AccountList.Insert( FAccountMeta( signers[i].PublicKey.GetKeyData(), true, true), i);
	//}
}

TArray<uint8> FTransaction::BuildMessage()
{
    TArray<uint8> accountKeysBuffer;
    for (FAccountMeta& accountMeta : AccountList)
    {
    	//Need to remove feepayer here????
        accountKeysBuffer.Append(accountMeta.PublicKey.GetKeyData());
		UpdateHeaderInfo(accountMeta);
    }

	TArray<uint8> buffer;
	buffer.Add(RequiredSignatures);
	buffer.Add(ReadOnlySignedAccounts);
	buffer.Add(ReadOnlyUnsignedAccounts);
	
	buffer.Append(FCryptoUtils::ShortVectorEncodeLength(AccountList.Num()));
	buffer.Append(accountKeysBuffer);

	buffer.Append( FBase58::DecodeBase58(BlockHash));

	TArray<uint8> compiledInstructions = CompileInstructions();
	buffer.Append(FCryptoUtils::ShortVectorEncodeLength(Instructions.Num()));
	buffer.Append(compiledInstructions);

	return buffer;
}

TArray<uint8> FTransaction::CompileInstructions()
{
	TArray<uint8> result;
	
	for (FInstructionData& instruction: Instructions)
	{
		const int keyCount = instruction.Keys.Num();
		TArray<uint8> keyIndices;
		keyIndices.SetNum(keyCount);
	
		for (int i = 0; i < keyCount; i++)
		{
			keyIndices[i] = GetAccountIndex(instruction.Keys[i].PublicKey.GetKey());
		}

		result.Add(GetAccountIndex(instruction.ProgramId.GetKey()));
		result.Append(FCryptoUtils::ShortVectorEncodeLength(keyCount));
		result.Append(keyIndices);
		result.Append(FCryptoUtils::ShortVectorEncodeLength(instruction.Data.Num()));
		result.Append(instruction.Data);
	}

	return result;
}

void FTransaction::UpdateHeaderInfo(const FAccountMeta& accountMeta)
{
	if (accountMeta.Signer)
	{
		RequiredSignatures += 1;
		if (!accountMeta.Writable)
			ReadOnlySignedAccounts += 1;
	}
	else 
	{
		if (!accountMeta.Writable)
			ReadOnlyUnsignedAccounts += 1;
	}
}

TArray<uint8> FTransaction::Sign(const TArray<uint8>& message, const TArray<FAccount>& signers)
{
	TArray<uint8> signatures;

	signatures.Append(FCryptoUtils::ShortVectorEncodeLength(signers.Num()));
	
	for(FAccount signer: signers)
	{
		signatures.Append( signer.Sign(message) );
	}

	return signatures;
}