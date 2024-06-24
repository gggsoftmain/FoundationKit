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

#include "Nft/NFTMetadata.h"

FNFTCreator::FNFTCreator(FPublicKey key, uint8 share, bool verified)
    :PublicKey(key), Share(share), Verified(verified)
{
}

TArray<uint8> FNFTCreator::Encode() 
{
    TArray<uint8> data;

    data.Append(PublicKey.GetKeyData());
    data.Add(Verified ? 1 : 0);
    data.Add(Share);
    check(data.Num() == 34);
    return data;
}

TArray<uint8> FNFTCreator::Encode() const
{
    TArray<uint8> data;

    data.Append(PublicKey.GetKeyData());
    data.Add(Verified ? 1 : 0);
    data.Add(Share);
    check(data.Num() == 34);
    return data;
}
