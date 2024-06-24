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

#include "SolanaUtils/Program/CommonPrograms.h"

const FName FSystemProgram::ProgramIdName("11111111111111111111111111111111");
const FPublicKey FSystemProgram::ProgramIdKey("11111111111111111111111111111111");

const FName FTokenProgram::ProgramIdName("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");
const FPublicKey FTokenProgram::ProgramIdKey("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA");

const FName FATAProgram::ProgramIdName("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");
const FPublicKey FATAProgram::ProgramIdKey("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL");

FPublicKey FATAProgram::GetAssociatedTokenAddress(const FPublicKey& owner, const FPublicKey& mint, bool allowOwnerOffCurve, const FPublicKey& programId, const FPublicKey& associatedTokenProgramId)
{
	FPublicKey publicKey;
	if(allowOwnerOffCurve == false && owner.IsOnCurve() == false)
	{
		return publicKey;
	}

	TArray<TArray<uint8>> seeds;
	seeds.Add(owner.GetKeyData());
	seeds.Add(programId.GetKeyData());
	seeds.Add(mint.GetKeyData());

	if (FPublicKey::FindProgramAddress(seeds, associatedTokenProgramId, publicKey) == false) 
	{
		return publicKey;
	}

	return publicKey;
}

const FName FMetadataProgram::ProgramIdName("metaqbxxUerdq28cj1RbAWkYQm3ybzjb6a8bt518x1s");
const FPublicKey FMetadataProgram::ProgramIdKey("metaqbxxUerdq28cj1RbAWkYQm3ybzjb6a8bt518x1s");