// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameTypes.h"
#include "VCTelemetryTypes.h"
//#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpService.h"

#include "AVCTelemetryHttp.generated.h"



UCLASS()
class VITACORE_API AVCTelemetryHttp : public AHttpService
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void PostSingleRecord(FTelemetryRecord TelemetryRecord);

	void GetPostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
public:
	AVCTelemetryHttp();

	



};
