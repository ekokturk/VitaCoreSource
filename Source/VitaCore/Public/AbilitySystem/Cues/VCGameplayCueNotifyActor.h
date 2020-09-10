// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "VCGameplayCueNotifyActor.generated.h"

/**
 * This is a base class for Gameplay Cue Actors
 */

UCLASS()
class VITACORE_API AVCGameplayCueNotifyActor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	AVCGameplayCueNotifyActor();
};
