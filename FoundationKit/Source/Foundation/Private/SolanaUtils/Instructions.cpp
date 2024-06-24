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

#include "Instructions.h"

#include "SolanaUtils/Account.h"
#include "Crypto/Base58.h"
#include "Crypto/CryptoUtils.h"
#include "SolanaUtils/Utils/Types.h"
#include "SolanaUtils/Program/CommonPrograms.h"

const FPublicKey SysvarRentPublicKey("SysvarRent111111111111111111111111111111111");

FInstructionData FInstruction::TransferLamports(const FAccount& from, const FAccount& to, int64 lamports)
{
	FInstructionData result;

	result.ProgramId = FSystemProgram::ProgramIdKey;
	
	result.Keys.Add(FAccountMeta( from.PublicKey, true, true));
	result.Keys.Add(FAccountMeta( to.PublicKey, false, true));

	result.Keys.Add(FAccountMeta( result.ProgramId, false, false));
	
	result.Data.Append(FCryptoUtils::Int32ToDataArray((int)ESystemInstruction::Transfer));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(lamports));
	
	return result;
}

FInstructionData FInstruction::CreateAccount(const FAccount& from, const FAccount& newAccount, int64 rent)
{
	FInstructionData result;

	result.ProgramId = FSystemProgram::ProgramIdKey;
	
	result.Keys.Add(FAccountMeta( from.PublicKey, true, true));
	result.Keys.Add(FAccountMeta( newAccount.PublicKey, true, true));
	
	result.Keys.Add(FAccountMeta( result.ProgramId, false, false));
	
	result.Data.Append(FCryptoUtils::Int32ToDataArray((int)ESystemInstruction::CreateAccount));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(rent));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(AccountDataSize));
	result.Data.Append(FTokenProgram::ProgramIdKey.GetKeyData());
	
	return result;
}

FInstructionData FInstruction::InitializeTokenAccount(const FAccount& account, const FPublicKey& mint, const FAccount& owner)
{
	FInstructionData result;

	result.ProgramId = FTokenProgram::ProgramIdKey;
	
	result.Keys.Add(FAccountMeta( account.PublicKey, false, true));
	result.Keys.Add(FAccountMeta( mint, false, false));
	result.Keys.Add(FAccountMeta( owner.PublicKey, false, false));
	result.Keys.Add(FAccountMeta( SysvarRentPublicKey, false, false));

	result.Keys.Add(FAccountMeta( result.ProgramId, false, false));
	
	result.Data.Add((int)ETokenInstruction::InitializeAccount);
	
	return result;
}

FInstructionData FInstruction::TransferTokens(const FAccount& from, const FAccount& to, const FAccount& owner, int64 amount)
{
	FInstructionData result;
	
	result.ProgramId = FTokenProgram::ProgramIdKey;
	
	result.Keys.Add(FAccountMeta( from.PublicKey, false, true));
	result.Keys.Add(FAccountMeta( to.PublicKey, false, true));
	result.Keys.Add(FAccountMeta( owner.PublicKey, true, false));
	
	result.Keys.Add(FAccountMeta( result.ProgramId, false, false));

	//Finish this

	result.Data.Add((int)ETokenInstruction::Transfer);
	result.Data.Append(FCryptoUtils::Int64ToDataArray(amount));
	
	return result;
}


FInstructionData FInstruction::CreateSystemAccount(const FAccount& from, const FAccount& newAccount, int64 rent, int64 accountDataSize)
{
	FInstructionData result;

	result.ProgramId = FSystemProgram::ProgramIdKey;

	result.Keys.Add(FAccountMeta(from.PublicKey, true, true));
	result.Keys.Add(FAccountMeta(newAccount.PublicKey, true, true));

	result.Keys.Add(FAccountMeta(result.ProgramId, false, false));

	result.Data.Append(FCryptoUtils::Int32ToDataArray((int)ESystemInstruction::CreateAccount));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(rent));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(accountDataSize));
	result.Data.Append(FTokenProgram::ProgramIdKey.GetKeyData());

	return result;
}

FInstructionData FInstruction::CreateSystemAccount(const FPublicKey& from, const FPublicKey& newAccount, int64 rent, int64 accountDataSize, const FPublicKey& programId)
{
	FInstructionData result;

	result.Keys.Add(FAccountMeta(from, true, true));
	result.Keys.Add(FAccountMeta(newAccount, true, true));

	result.ProgramId = FSystemProgram::ProgramIdKey;

	result.Data.Append(FCryptoUtils::Int32ToDataArray((int)ESystemInstruction::CreateAccount));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(rent));
	result.Data.Append(FCryptoUtils::Int64ToDataArray(accountDataSize));
	result.Data.Append(programId.GetKeyData());

	return result;
}

FInstructionData FInstruction::TokenProgramInitializeMint(const FPublicKey& mint, int decimals, const FPublicKey& mintAuthority, const FPublicKey* freezeAuthority)
{
	FInstructionData result;

	result.Keys.Add(FAccountMeta(mint, false, true));
	result.Keys.Add(FAccountMeta(SysvarRentPublicKey, false, false));

	result.ProgramId = FTokenProgram::ProgramIdKey;

	int freezeAuthorityOpt = freezeAuthority ? 1 : 0;
	result.Data.Add((uint8)ETokenInstruction::InitializeMint);
	result.Data.Add((uint8)decimals);
	result.Data.Append(mintAuthority.GetKeyData());
	result.Data.Add((uint8)freezeAuthorityOpt);
	result.Data.Append(freezeAuthority? freezeAuthority->GetKeyData() : FAccount::Create().PublicKey.GetKeyData());

	return result;
}

FInstructionData FInstruction::TokenProgramMintTo(const FPublicKey& mint, const FPublicKey& destination, int64 amount, const FPublicKey& mintAuthority)
{
	FInstructionData result;

	result.Keys.Add(FAccountMeta(mint, false, true));
	result.Keys.Add(FAccountMeta(destination, false, true));
	result.Keys.Add(FAccountMeta(mintAuthority, true, false));

	result.ProgramId = FTokenProgram::ProgramIdKey;

	result.Data.Add((uint8)ETokenInstruction::MintTo);
	result.Data.Append(FCryptoUtils::Int64ToDataArray(amount));

	return result;
}

FInstructionData FInstruction::CreateAssociatedTokenAccount(const FPublicKey& payer, const FPublicKey& owner, const FPublicKey& mint)
{
	FPublicKey associatedToken = FATAProgram::GetAssociatedTokenAddress(owner, mint);

	FInstructionData result;

	result.Keys.Add(FAccountMeta(payer, true, true));
	result.Keys.Add(FAccountMeta(associatedToken, false, true));
	result.Keys.Add(FAccountMeta(owner, false, false));
	result.Keys.Add(FAccountMeta(mint, false, false));
	result.Keys.Add(FAccountMeta(FSystemProgram::ProgramIdKey, false, false));
	result.Keys.Add(FAccountMeta(FTokenProgram::ProgramIdKey, false, false));
	result.Keys.Add(FAccountMeta(SysvarRentPublicKey, false, false));

	result.ProgramId = FATAProgram::ProgramIdKey;

	return result;
}

FInstructionData FInstruction::CreateMetadataAccount(const FPublicKey& metadataPDA, const FPublicKey& mint, const FPublicKey& authority, 
	const FPublicKey& payer, const FPublicKey& updateAuthority, const FNFTMetadata& data, bool isMutable, bool updateAuthorityIsSigner, uint64 collectionDetails)
{
	FInstructionData result;
	
	result.Keys.Add(FAccountMeta(metadataPDA, false, true));
	result.Keys.Add(FAccountMeta(mint, false, false));
	result.Keys.Add(FAccountMeta(authority, true, false));
	result.Keys.Add(FAccountMeta(payer, true, true));
	result.Keys.Add(FAccountMeta(updateAuthority, updateAuthorityIsSigner, false));
	result.Keys.Add(FAccountMeta(FSystemProgram::ProgramIdKey, false, false));
	result.Keys.Add(FAccountMeta(SysvarRentPublicKey, false, false));

	result.ProgramId = FMetadataProgram::ProgramIdKey;

	result.Data.Add(33);	// CreateMetadataAccountV3
	
	TArray<uint8> strdata = FCryptoUtils::FStringToUint8(data.Name);
	result.Data.Append(FCryptoUtils::Int32ToDataArray(strdata.Num()));
	result.Data.Append(strdata);

	strdata = FCryptoUtils::FStringToUint8(data.Symbol);
	result.Data.Append(FCryptoUtils::Int32ToDataArray(strdata.Num()));
	result.Data.Append(strdata);

	strdata = FCryptoUtils::FStringToUint8(data.Uri);
	result.Data.Append(FCryptoUtils::Int32ToDataArray(strdata.Num()));
	result.Data.Append(strdata);

	result.Data.Append(FCryptoUtils::ShortToDataArray((short)data.SellerFeeBasisPoints));

	result.Data.Add(1);	// creators
	result.Data.Append(FCryptoUtils::Int32ToDataArray(data.Creators.Num()));
	for (int i = 0; i < data.Creators.Num(); ++i) 
	{
		result.Data.Append(data.Creators[i].Encode());
	}

	result.Data.Add(0);	// No Collection link
	result.Data.Add(0);	// Not consumable

	result.Data.Add(isMutable?1:0);

	if (collectionDetails == 0) 
	{
		result.Data.Add(0);	// No collection details
	}
	else
	{
		result.Data.Add(1);
		result.Data.Add(0);
		result.Data.Append(FCryptoUtils::Int64ToDataArray(collectionDetails));
	}
	
	return result;
}

FInstructionData FInstruction::CreateMasterEdition(const FPublicKey& masterEditionPAD, const FPublicKey& mint, const FPublicKey& payer, 
	const FPublicKey& updateAuthority, const FPublicKey& mintAuthority, const FPublicKey& metadataPDA)
{
	FInstructionData result;

	result.Keys.Add(FAccountMeta(masterEditionPAD, false, true));
	result.Keys.Add(FAccountMeta(mint, false, true));
	result.Keys.Add(FAccountMeta(updateAuthority, true, false));
	result.Keys.Add(FAccountMeta(mintAuthority, true, false));
	result.Keys.Add(FAccountMeta(payer, true, false));
	result.Keys.Add(FAccountMeta(metadataPDA, false, false));
	result.Keys.Add(FAccountMeta(FTokenProgram::ProgramIdKey, false, false));
	result.Keys.Add(FAccountMeta(FSystemProgram::ProgramIdKey, false, false));
	result.Keys.Add(FAccountMeta(SysvarRentPublicKey, false, false));

	result.ProgramId = FMetadataProgram::ProgramIdKey;

	result.Data.Add(17);	// CreateMasterEditionV3
	result.Data.Add(0);
	return result;
}