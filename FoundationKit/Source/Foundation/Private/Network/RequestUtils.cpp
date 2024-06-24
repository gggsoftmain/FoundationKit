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

#include "Network/RequestUtils.h"

#include "JsonObjectConverter.h"
#include "Network/RequestManager.h"
#include "Misc/MessageDialog.h"
#include "Misc/EngineVersionComparison.h"
#include "SolanaUtils/Utils/Types.h"



static FText ErrorTitle = FText::FromString("Error");
static FText InfoTitle = FText::FromString("Info");

FRequestData* FRequestUtils::RequestAccountInfo(const FString& pubKey)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());

	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%u,"method":"getAccountInfo","params":["%s",{"encoding": "base58"}]})")
		,request->Id, *pubKey );

	return request;
}

FAccountInfoJson FRequestUtils::ParseAccountInfoResponse(const FJsonObject& data)
{
	FAccountInfoJson jsonData;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> resultData = result->GetObjectField("value");

		FString OutputString;
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(resultData.ToSharedRef(), Writer);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, OutputString);

		FJsonObjectConverter::JsonObjectToUStruct(resultData.ToSharedRef(), &jsonData);

		FString Out = FString::Printf(TEXT("lamports: %f, owner: %s, rentEpoch: %i"), jsonData.lamports, *jsonData.owner, jsonData.rentEpoch);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, Out);

	}
	return jsonData;
}

FRequestData* FRequestUtils::RequestAccountBalance(const FString& pubKey)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getBalance","params":["%s",{"commitment": "processed"}]})")
			,request->Id , *pubKey );
	
	return request;
}


double FRequestUtils::ParseAccountBalanceResponse(const FJsonObject& data)
{
	FBalanceResultJson jsonData;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonObjectToUStruct(result.ToSharedRef(), &jsonData);
		return jsonData.value;
	}
	return -1;
}

FRequestData* FRequestUtils::RequestTokenAccount(const FString& pubKey, const FString& mint)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"mint": "%s"},{"encoding": "jsonParsed"}]})")
			,request->Id, *pubKey, *mint );
	
	return request;
}

FString FRequestUtils::ParseTokenAccountResponse(const FJsonObject& data)
{
	FTokenAccountArrayJson jsonData = ParseAllTokenAccountsResponse(data);

	FString result;
	if( !jsonData.value.IsEmpty() )
	{
		result = jsonData.value[0].pubkey;
	}
	return result;
}

FRequestData* FRequestUtils::RequestAllTokenAccounts(const FString& pubKey, const FString& programID)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getTokenAccountsByOwner","params":["%s",{"programId": "%s"},{"encoding": "jsonParsed"}]})")
			,request->Id, *pubKey, *programID );
	
	return request;
}

FTokenAccountArrayJson FRequestUtils::ParseAllTokenAccountsResponse(const FJsonObject& data)
{
	FTokenAccountArrayJson jsonData;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonObjectToUStruct(result.ToSharedRef(), &jsonData);
	}
	return jsonData;
}

FRequestData* FRequestUtils::RequestProgramAccounts(const FString& programID, const UINT& size, const FString& pubKey)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getProgramAccounts","params":["%s",{"encoding":"base64","filters":[{"dataSize":%d},{"memcmp":{"offset":8,"bytes":"%s"}}]}]})")
			,request->Id, *programID, size,*pubKey );
		
	return request;
}

TArray<FProgramAccountJson> FRequestUtils::ParseProgramAccountsResponse(const FJsonObject& data)
{
	TArray<FProgramAccountJson> list;
	TArray<TSharedPtr<FJsonValue>> dataArray = data.GetArrayField("result");
	for(const TSharedPtr<FJsonValue> entry:  dataArray )
	{
		const TSharedPtr<FJsonObject> entryObject = entry->AsObject();
		if( TSharedPtr<FJsonObject> account = entryObject->GetObjectField("account") )
		{
			FProgramAccountJson accountData;
			FJsonObjectConverter::JsonObjectToUStruct( account.ToSharedRef() , &accountData);
			list.Add(accountData);
		}
		FString pubKey = entryObject->GetStringField("pubkey");
	}
	
	return list;
}

FRequestData* FRequestUtils::RequestMultipleAccounts(const TArray<FString>& pubKey)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	FString list;
	for( FString key: pubKey )
	{
		list.Append(FString::Printf(TEXT(R"("%s")"),*key));

		if(key != pubKey.Last())
		{
			list.Append(",");
		}
	}
		
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method": "getMultipleAccounts","params":[[%s],{"dataSlice":{"offset":0,"length":0}}]})")
			,request->Id , *list );
	
	return request;
}

TArray<FAccountInfoJson> FRequestUtils::ParseMultipleAccountsResponse(const FJsonObject& data)
{
	TArray<FAccountInfoJson> jsonData;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		FJsonObjectConverter::JsonArrayToUStruct( result->GetArrayField("value") , &jsonData);
	}
	return jsonData;
}

FRequestData* FRequestUtils::SendTransaction(const FString& transaction)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"sendTransaction","params":["%s",{"encoding": "base64", "preflightCommitment": "confirmed"}]})")
			,request->Id, *transaction );
	
	return request;
}

FString FRequestUtils::ParseTransactionResponse(const FJsonObject& data)
{
	return data.GetStringField("result");
}

FRequestData* FRequestUtils::RequestBlockHash()
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"id":%d,"jsonrpc":"2.0","method":"getLatestBlockhash","params":[{"commitment": "finalized"}]})")
				,request->Id );
	
	return request;
}

FString FRequestUtils::ParseBlockHashResponse(const FJsonObject& data)
{
	FString hash;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> value = result->GetObjectField("value");
		hash = value->GetStringField("blockhash");
	}
	return hash;
}

FRequestData* FRequestUtils::GetTransactionFeeAmount(const FString& transaction)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getFeeForMessage", "params":[%s,{"commitment":"processed"}]})")
			,request->Id, *transaction );
	
	return request;
}

int FRequestUtils::ParseTransactionFeeAmountResponse(const FJsonObject& data)
{
	int fee = 0;
	if(TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		const TSharedPtr<FJsonObject> value = result->GetObjectField("value");
		fee = value->GetNumberField("value");
	}
	return fee;
}

FRequestData* FRequestUtils::GetMinimumBalanceForRentExemption(const UINT& size)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());

	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d,"method":"getMinimumBalanceForRentExemption", "params":[%d, {"commitment": "finalized"}]})")
			, request->Id, size);

	return request;
}

int64 FRequestUtils::ParseMinimumBalanceForRentExemptionResponse(const FJsonObject& data)
{
	int64 value;
	data.TryGetNumberField("result", value);
	return value;
}

FRequestData* FRequestUtils::RequestAirDrop(const FString& pubKey)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());
	
	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d, "method":"requestAirdrop", "params":["%s", 1000000000]})")
				,request->Id, *pubKey );
	
	return request;
}

FRequestData* FRequestUtils::GetSignatureStatuses(const FString& hash)
{
	FRequestData* request = new FRequestData(FRequestManager::GetNextMessageID());

	request->Body =
		FString::Printf(TEXT(R"({"jsonrpc":"2.0","id":%d, "method":"getSignatureStatuses", "params":[["%s"], {"searchTransactionHistory": false} ]})")
			, request->Id, *hash);

	return request;
}

bool FRequestUtils::ParseGetSignatureStatusesResponse(const FJsonObject& data)
{
	if (TSharedPtr<FJsonObject> result = data.GetObjectField("result"))
	{
		const TArray<TSharedPtr<FJsonValue>>& values = result->GetArrayField("value");
		if (values.Num() > 0)
		{
			const TSharedPtr<FJsonObject>* valueObj = nullptr;
			if (values[0]->TryGetObject(valueObj))
			{
				FString status;
				if ((*valueObj)->TryGetStringField(TEXT("confirmationStatus"), status))
				{
					if (status == "finalized")
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

class FConfirmTransactionUtils
{
public:
	static const float TIME_OUT;

	FConfirmTransactionUtils(FString hash, FDateTime startTime, FConfirmTransactionCallback callback)
		:Hash(hash), StartTime(startTime), Callback(callback)
	{
	}

	FString Hash;
	FDateTime StartTime;
	FConfirmTransactionCallback Callback;

	static void SendGetSignatureStatuses(UWorld* world, FConfirmTransactionUtils ctu)
	{
		FRequestData* request = FRequestUtils::GetSignatureStatuses(ctu.Hash);
		request->Callback.BindLambda([ctu, world](const FJsonObject& data)
		{
			bool ret = FRequestUtils::ParseGetSignatureStatusesResponse(data);
			if (ret == true)
			{
				ctu.Callback.Execute(true);
				return;
			}
			const FTimespan  passedTime = FDateTime::Now() - ctu.StartTime;
			if (passedTime.GetTotalSeconds() >= FConfirmTransactionUtils::TIME_OUT)
			{
				ctu.Callback.Execute(false);
				return;
			}

			FTimerHandle handle;
			world->GetTimerManager().SetTimer(handle, [world, ctu]()
			{
				FConfirmTransactionUtils::SendGetSignatureStatuses(world, ctu);
			}, 5.0f, false);
		});

		request->ErrorCallback.BindLambda([ctu](const FString& FailureReason)
		{
			ctu.Callback.Execute(false);
		});
		FRequestManager::SendRequest(request);
	}
};
const float FConfirmTransactionUtils::TIME_OUT = 60;

void FRequestUtils::ConfirmTransaction(UWorld* world, const FString& hash, FConfirmTransactionCallback callback)
{
	FConfirmTransactionUtils::SendGetSignatureStatuses(world, FConfirmTransactionUtils(hash, FDateTime::Now(), callback));
}

void FRequestUtils::DisplayError(const FString& error)
{
#if UE_VERSION_NEWER_THAN(5, 3, 0)
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(error), ErrorTitle);
#else
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(error), &ErrorTitle);
#endif
}

void FRequestUtils::DisplayInfo(const FString& info)
{
#if UE_VERSION_NEWER_THAN(5, 3, 0)
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(info), InfoTitle);
#else
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(info), &InfoTitle);
#endif
}
