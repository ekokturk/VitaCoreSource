// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineDefines.h"

VITACOREEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogVitaCoreEditor, Display, All);

#define VCEDITOR_LOG(Verbosity, Format, ...) UE_LOG(LogVitaCoreEditor, Verbosity, Format, ##__VA_ARGS__)