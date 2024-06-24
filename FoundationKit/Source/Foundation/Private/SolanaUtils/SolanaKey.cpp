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

#include "SolanaUtils/SolanaKey.h"
#include "SolanaUtils/Program/CommonPrograms.h"
#include "Crypto/CryptoUtils.h"
#include "Crypto/Base58.h"
#include <openssl/e_os2.h>

void FSolanaKey::SetKey(FString key)
{
    Key = key;
    KeyData = FBase58::DecodeBase58(Key);
    KeyData.AddZeroed(GetKeyLength() - KeyData.Num());

    //check(KeyData.Num() == GetKeyLength());
    IsValid = true;
}

void FSolanaKey::SetKeyData(TArray<uint8>& keyData)
{
    KeyData = keyData;
    Key = FBase58::EncodeBase58(keyData.GetData(), keyData.Num());
    check(KeyData.Num() == GetKeyLength());
    IsValid = true;
}

void FSolanaKey::SetKeyData(const TArray<uint8>& keyData)
{
    KeyData = keyData;
    Key = FBase58::EncodeBase58(keyData.GetData(), keyData.Num());
    check(KeyData.Num() == GetKeyLength());
    IsValid = true;
}

bool FSolanaKey::GenerateKeyPair(const TArray<uint8>& Seed, FPublicKey& publicKey, FPrivateKey& privateKey)
{
    FCryptoUtils::GenerateKeyPair(Seed, publicKey.KeyData, privateKey.KeyData);

    publicKey.Key = FBase58::EncodeBase58(publicKey.KeyData.GetData(), publicKey.KeyData.Num());
    publicKey.IsValid = true;

    privateKey.Key = FBase58::EncodeBase58(privateKey.KeyData.GetData(), privateKey.KeyData.Num());
    privateKey.IsValid = true;
    return true;
}


FPublicKey::FPublicKey()
{
    KeyData.SetNum(FPublicKey::GetKeyLengthStatic());
}

FPublicKey::FPublicKey(FString key)
{
    KeyData.SetNum(FPublicKey::GetKeyLengthStatic());
    SetKey(key);
}

FPublicKey::~FPublicKey()
{
}

void FPublicKey::SetKeyFromPrivate(const FPrivateKey& privateKey)
{
    const TArray<uint8>& privateKeyData = privateKey.GetKeyData();
    for (int i = 0; i < PublicKeySize; i++)
    {
        KeyData[i] = privateKeyData[i + PublicKeySize];
    }

    Key = FBase58::EncodeBase58(KeyData.GetData(), KeyData.Num());
}

bool FPublicKey::IsOnCurve() const
{
    return FCryptoUtils::IsOnCurve(KeyData);
}

FPublicKey FPublicKey::FindMetadataPDA(const FPublicKey& mintAddress)
{
    TArray<TArray<uint8>> seeds;

    seeds.Add({ 'm', 'e', 't', 'a', 'd', 'a', 't', 'a' });
    seeds.Add(FMetadataProgram::ProgramIdKey.GetKeyData());
    seeds.Add(mintAddress.GetKeyData());

    FPublicKey address;
    FindProgramAddress(seeds, FMetadataProgram::ProgramIdKey, address);
    return address;
}

FPublicKey FPublicKey::FindMasterEditionPDA(const FPublicKey& mintAddress)
{
    TArray<TArray<uint8>> seeds;

    seeds.Add({ 'm', 'e', 't', 'a', 'd', 'a', 't', 'a' });
    seeds.Add(FMetadataProgram::ProgramIdKey.GetKeyData());
    seeds.Add(mintAddress.GetKeyData());
    seeds.Add({ 'e', 'd', 'i', 't', 'i', 'o', 'n' });

    FPublicKey address;
    FindProgramAddress(seeds, FMetadataProgram::ProgramIdKey, address);
    return address;
}

bool FPublicKey::FindProgramAddress(const TArray<TArray<uint8>>& seeds, const FPublicKey& programId, FPublicKey& outPublicKey)
{
    uint8 seedBump = 255;
    TArray<TArray<uint8>> buffer = seeds;
    TArray<uint8> bumpArray = { seedBump };
    buffer.Add(bumpArray);

    while (seedBump != 0)
    {
        bumpArray[0] = seedBump;
        buffer[buffer.Num() - 1] = bumpArray;

        FPublicKey derivedAddress;
        bool success = CreateProgramAddress(buffer, programId, derivedAddress);

        if (success)
        {
            outPublicKey = derivedAddress;
            return true;
        }

        seedBump--;
    }

    return false;
}

bool FPublicKey::CreateProgramAddress(const TArray<TArray<uint8>>& seeds, const FPublicKey& programId, FPublicKey& outPublicKey)
{
    TArray<uint8> Buffer;

    for (const TArray<uint8>& Seed : seeds)
    {
        if (Seed.Num() > GetKeyLengthStatic())
        {
            UE_LOG(LogTemp, Error, TEXT("Max seed length exceeded"));
            return false;
        }
        Buffer.Append(Seed);
    }

    Buffer.Append(programId.GetKeyData());
    // ProgramDerivedAddress
    static TArray<uint8> PDAString = { 'P', 'r', 'o', 'g', 'r', 'a', 'm', 'D', 'e', 'r', 'i', 'v', 'e', 'd', 'A', 'd', 'd', 'r', 'e', 's', 's' };
    Buffer.Append(PDAString);

    TArray<uint8> HashedBuffer = FCryptoUtils::SHA256_Digest(Buffer.GetData(), Buffer.Num());

    if (FCryptoUtils::IsOnCurve(HashedBuffer))
    {
        return false;
    }

    // Convert PublicKeyBytes to Base64 for example
    outPublicKey.SetKeyData(HashedBuffer);
    return true;
}

/////////////////////// FPrivateKey////////////////////////
FPrivateKey::FPrivateKey()
{
    KeyData.SetNum(FPrivateKey::GetKeyLengthStatic());
}

FPrivateKey::~FPrivateKey()
{
}
