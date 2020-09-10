// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "AVCTelemetryHttp.h"
#include "HttpService.h"

AVCTelemetryHttp::AVCTelemetryHttp()
{

}

void AVCTelemetryHttp::BeginPlay() {
	Super::BeginPlay();

	FTelemetryRecord LoginCredentials;
	LoginCredentials.sessionId = 31;
	LoginCredentials.gameAction = 31;
	LoginCredentials.xCord = 31;
	LoginCredentials.yCord = 31;
	LoginCredentials.id = 0;
	LoginCredentials.cmd = TEXT("save_record");
	PostSingleRecord(LoginCredentials);
}

void AVCTelemetryHttp::PostSingleRecord(FTelemetryRecord TelemetryRecord)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FTelemetryRecord>(TelemetryRecord, ContentJsonString);

	UE_LOG(LogTemp, Log, TEXT("%s"), *ContentJsonString);
	
	TSharedRef<IHttpRequest> Request = PostRequest("api/sessions", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AVCTelemetryHttp::GetPostResponse);
	Send(Request);
}

void AVCTelemetryHttp::GetPostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FTelemetry_Response TelemetryResponse;
	GetStructFromJsonString<FTelemetry_Response>(Response, TelemetryResponse);

	UE_LOG(LogTemp, Warning, TEXT("Error is: %i"), TelemetryResponse.returnCode);
}