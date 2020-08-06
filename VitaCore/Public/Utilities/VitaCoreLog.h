// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineDefines.h"

VITACORE_API DECLARE_LOG_CATEGORY_EXTERN(LogVitaCore, Display, All);

#define VC_LOG(Verbosity, Format, ...) UE_LOG(LogVitaCore, Verbosity, TEXT(Format), ##__VA_ARGS__)