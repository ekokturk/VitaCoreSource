// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCMenuHttp.h"
#include "Kismet/GameplayStatics.h"

AVCMenuHttp::AVCMenuHttp()
{
	ApiBaseUrl = "http://localhost:3000/";
	//ApiBaseUrl = "http://192.168.1.159:3000/";
	//ApiBaseUrl = "http://10.128.172.82:3000/";
}

void AVCMenuHttp::SetBaseUrl(FString Url)
{
	IP = Url;
	ApiBaseUrl = "http://" + Url + ":3000/";
}

//------- Enter Queue

void AVCMenuHttp::PostEnterQueue(FClientEnterQueue_Request EnterQueueRequest)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FClientEnterQueue_Request>(EnterQueueRequest, ContentJsonString);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("api/queue/add", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AVCMenuHttp::GetEnterQueueResponse);
	Send(Request);
}

void AVCMenuHttp::GetEnterQueueResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FClientEnterQueue_Response EnterQueueResponse;
	GetStructFromJsonString<FClientEnterQueue_Response>(Response, EnterQueueResponse);

	UE_LOG(LogTemp, Warning, TEXT("Error is: %i"), EnterQueueResponse.error);
}

//------- Exit Queue

void AVCMenuHttp::PostExitQueue(FClientExitQueue_Request ExitQueueRequest)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FClientExitQueue_Request>(ExitQueueRequest, ContentJsonString);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("api/queue/remove", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AVCMenuHttp::GetExitQueueResponse);
	Send(Request);
}

void AVCMenuHttp::GetExitQueueResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FClientExitQueue_Response EnterQueueResponse;
	GetStructFromJsonString<FClientExitQueue_Response>(Response, EnterQueueResponse);

	UE_LOG(LogTemp, Warning, TEXT("Error is: %i"), EnterQueueResponse.error);
}

//------- Check Session

void AVCMenuHttp::PostCheckSession(FClientCheckSession_Request CheckSessionRequest)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FClientCheckSession_Request>(CheckSessionRequest, ContentJsonString);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("api/sessions/check", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AVCMenuHttp::GetCheckSessionResponse);
	Send(Request);
}


void AVCMenuHttp::GetCheckSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FClientCheckSession_Response CheckSessionResponse;
	GetStructFromJsonString<FClientCheckSession_Response>(Response, CheckSessionResponse);

	UE_LOG(LogTemp, Warning, TEXT("Error is: %i"), CheckSessionResponse.error);
	if (CheckSessionResponse.error > 0)
	{
		return;
	}

	UWorld * World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FString Level = IP + ":";
	//FString Level = "127.0.0.1:";
	//FString Level = "10.128.172.82:";
	Level.Append(FString::FromInt(CheckSessionResponse.session));
	UGameplayStatics::OpenLevel(World, FName(*Level));
}



