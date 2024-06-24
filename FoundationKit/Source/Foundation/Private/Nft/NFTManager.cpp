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

#include "Nft/NFTManager.h"
#include "WalletAccount.h"
#include "Network/RequestUtils.h"
#include "Network/RequestManager.h"
#include "SolanaUtils/Utils/TransactionUtils.h"
#include "SolanaUtils/Utils/Types.h"
#include "SolanaUtils/Program/CommonPrograms.h"
#include "SolanaWallet.h"

void UNFTManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNFTManager::CreateNFT(const FAccount& owner, FNFTMetadata& metadata, FCreateNFTCallback callBack)
{
    OwnerAccount = owner;
    Metadata = metadata;
    CreateNFTCallback = callBack;

    MintToken = FAccount::Create();
    AssociatedTokenAccount = FATAProgram::GetAssociatedTokenAddress(OwnerAccount.PublicKey, MintToken.PublicKey);
    if (AssociatedTokenAccount.GetIsValid() == false)
    {
        OnErrorCallback("Failed to get associated token account.");
        return;
    }

    FRequestData* blockhashRequest = FRequestUtils::RequestBlockHash();
    blockhashRequest->Callback.BindUObject(this, &UNFTManager::OnRequestBlockHash);
    blockhashRequest->ErrorCallback.BindUObject(this, &UNFTManager::OnErrorCallback);

    FRequestManager::SendRequest(blockhashRequest);
}

void UNFTManager::OnRequestBlockHash(FJsonObject& responseData)
{
    BlockHash = FRequestUtils::ParseBlockHashResponse(responseData);
    FRequestData* minimumBalanceRequest = FRequestUtils::GetMinimumBalanceForRentExemption(MintAccountDataSize);
    minimumBalanceRequest->Callback.BindUObject(this, &UNFTManager::OnGetMinimumBalanceForRentExemption);
    minimumBalanceRequest->ErrorCallback.BindUObject(this, &UNFTManager::OnErrorCallback);

    FRequestManager::SendRequest(minimumBalanceRequest);
}

void UNFTManager::OnGetMinimumBalanceForRentExemption(FJsonObject& responseData)
{
    MinimumRent = FRequestUtils::ParseMinimumBalanceForRentExemptionResponse(responseData);

    const TArray<uint8> transaction = FTransactionUtils::MintingTransaction(OwnerAccount, MintToken, AssociatedTokenAccount, MinimumRent, BlockHash, Metadata);

    FRequestData* sendTransaction = FRequestUtils::SendTransaction(FBase64::Encode(transaction));
    sendTransaction->Callback.BindUObject(this, &UNFTManager::OnSendTransaction);
    sendTransaction->ErrorCallback.BindUObject(this, &UNFTManager::OnErrorCallback);
    FRequestManager::SendRequest(sendTransaction);
}

void UNFTManager::OnSendTransaction(FJsonObject& responseData)
{
    FString hash = FRequestUtils::ParseTransactionResponse(responseData);
    FRequestUtils::ConfirmTransaction(GetWorld(), hash, FConfirmTransactionCallback::CreateUObject(this, &UNFTManager::OnResponseConfirmTransaction));
}

void UNFTManager::OnResponseConfirmTransaction(bool success)
{
    CreateNFTCallback.Execute(success, MintToken);
}

void UNFTManager::OnErrorCallback(const FString& failureReason)
{
    CreateNFTCallback.Execute(false, MintToken);
}