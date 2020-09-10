// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "ScriptMacros.h"
#include "VCTelemetryTypes.generated.h"


USTRUCT()
struct FTelemetryRecord 
{
	GENERATED_BODY()

	UPROPERTY()	int sessionId;
	UPROPERTY() int gameAction;
	UPROPERTY() int xCord;
	UPROPERTY() int yCord;
	UPROPERTY() int id;
	UPROPERTY() FString cmd;

	FTelemetryRecord() {}
};

USTRUCT()
struct FTelemetry_Response
{
	GENERATED_BODY()

		UPROPERTY() int returnCode;

	FTelemetry_Response() {}
};
