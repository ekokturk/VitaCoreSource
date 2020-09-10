// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json\Public\Serialization\JsonSerializer.h"
#include "Json.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "HttpService.generated.h"




UCLASS(ABSTRACT, hideCategories = (Rendering, Replication, Input, Actor, "Actor Tick"))
	class VITACORE_API AHttpService : public AActor
{
	GENERATED_BODY()
protected:

	FHttpModule* Http;
	FString ApiBaseUrl;


public:
	AHttpService();

	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	void Send(TSharedRef<IHttpRequest>& Request);



	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput)
	{
		FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
	}

	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput)
	{
		StructType StructData;
		FString JsonString = Response->GetContentAsString();
		FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
	}

};