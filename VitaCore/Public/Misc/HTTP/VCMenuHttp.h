// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HttpService.h"
#include "VCClientHTTPTypes.h"
#include "VCMenuHttp.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API AVCMenuHttp : public AHttpService
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		FString IP;

public:
	AVCMenuHttp();

	UFUNCTION(BlueprintCallable)
	void SetBaseUrl(FString Url);

	void PostEnterQueue(FClientEnterQueue_Request TelemetryRecord);
	void PostExitQueue(FClientExitQueue_Request TelemetryRecord);
	void PostCheckSession(FClientCheckSession_Request TelemetryRecord);

	void GetEnterQueueResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetExitQueueResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetCheckSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
