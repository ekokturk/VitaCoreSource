// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "ScriptMacros.h"
#include "VCClientHttpTypes.generated.h"


USTRUCT()
struct FClientEnterQueue_Request
{
	GENERATED_BODY()

	UPROPERTY()	FString player;
	UPROPERTY() FString game = "vitacore";

	FClientEnterQueue_Request() {}
};

USTRUCT()
struct FClientEnterQueue_Response
{
	GENERATED_BODY()

		UPROPERTY() int error;

	FClientEnterQueue_Response() {}
};


USTRUCT()
struct FClientExitQueue_Request
{
	GENERATED_BODY()

	UPROPERTY()	FString player;
	UPROPERTY() FString game = "vitacore";

	FClientExitQueue_Request() {}
};

USTRUCT()
struct FClientExitQueue_Response
{
	GENERATED_BODY()

		UPROPERTY() int error;

	FClientExitQueue_Response() {}
};


USTRUCT()
struct FClientCheckSession_Request
{
	GENERATED_BODY()

	UPROPERTY()	FString player;
	UPROPERTY() FString game = "vitacore";

	FClientCheckSession_Request() {}
};

USTRUCT()
struct FClientCheckSession_Response
{
	GENERATED_BODY()

	UPROPERTY() int error;
	UPROPERTY() int session;

	FClientCheckSession_Response() {}
};