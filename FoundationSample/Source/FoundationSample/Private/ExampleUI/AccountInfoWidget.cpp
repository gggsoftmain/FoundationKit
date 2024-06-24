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

#include "ExampleUI/AccountInfoWidget.h"
#include "Nft/NFTManager.h"
#include "SolanaWalletManager.h"
#include "WalletAccount.h"
#include "ExampleUI/LoadingSpinner.h"

void UAccountInfoWidget::OnStartMinting()
{
	UNFTManager* nftManager = GetGameInstance()->GetSubsystem<UNFTManager>();
	USolanaWalletManager* solanaWalletManager = GetGameInstance()->GetSubsystem<USolanaWalletManager>();
	if (nftManager == nullptr || solanaWalletManager == nullptr)
	{
		return;
	}
	
	USolanaWallet* solanaWallet = solanaWalletManager->GetOrCreateWallet("ExampleUIWallet");
	if(solanaWallet == nullptr)
	{
		return;
	}

	UWalletAccount* walletAccount = solanaWallet->GetAccountFromGenIndex(0);
	if (walletAccount == nullptr)
	{
		return;
	}

	ULoadingSpinner::StartLoading(this);

	FNFTMetadata metadata;
	metadata.Name = "Solana UE SDK NFT";
	metadata.Symbol = "GGSF";
	metadata.Uri = "https://raw.githubusercontent.com/gggsoftmain/ue_solana_sample/main/ExternalData/nft_metadata.json?token=GHSAT0AAAAAACS4BMH56FJUZHD6ZKZNMNHYZTTAIAA";
	metadata.SellerFeeBasisPoints = 0;
	metadata.Creators.Add(FNFTCreator(walletAccount->AccountData.PublicKey, 100, true));

	nftManager->CreateNFT(walletAccount->AccountData, metadata, FCreateNFTCallback::CreateUObject(this, &UAccountInfoWidget::OnCreateNFT));
}

void UAccountInfoWidget::OnCreateNFT(bool success, FAccount& nftToken)
{
	ULoadingSpinner::EndLoading();

	if (success)
	{
		USolanaWalletManager* solanaWalletManager = GetGameInstance()->GetSubsystem<USolanaWalletManager>();
		if (solanaWalletManager == nullptr)
		{
			return;
		}

		USolanaWallet* solanaWallet = solanaWalletManager->GetOrCreateWallet("ExampleUIWallet");
		if (solanaWallet == nullptr)
		{
			return;
		}

		solanaWallet->UpdateTokenAccounts();
	}
}