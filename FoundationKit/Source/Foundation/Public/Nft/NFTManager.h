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
#include "Subsystems/GameInstanceSubsystem.h"
#include "SolanaUtils/Account.h"
#include "Nft/NFTMetadata.h"
#include "NFTManager.generated.h"

DECLARE_DELEGATE_TwoParams(FCreateNFTCallback, bool, FAccount&);

UCLASS()
class FOUNDATION_API UNFTManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateNFT(const FAccount& owner, FNFTMetadata& metadata, FCreateNFTCallback callBack);

private:
	void OnRequestBlockHash(FJsonObject& responseData);
	void OnGetMinimumBalanceForRentExemption(FJsonObject& responseData);
	void OnSendTransaction(FJsonObject& responseData);
	void OnResponseConfirmTransaction(bool success);

	void OnErrorCallback(const FString& failureReason);
private:
	FCreateNFTCallback CreateNFTCallback;

	FAccount OwnerAccount;
	FAccount MintToken;
	FPublicKey AssociatedTokenAccount;
	FString BlockHash;
	int64 MinimumRent;
	FNFTMetadata Metadata;
};
