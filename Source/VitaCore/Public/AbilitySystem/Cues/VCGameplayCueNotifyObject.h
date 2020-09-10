// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "VCGameplayCueNotifyObject.generated.h"

/**
 * This is a base class for Gameplay Cue Objects 
 */

UCLASS()
class VITACORE_API UVCGameplayCueNotifyObject : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UVCGameplayCueNotifyObject();

protected:

};
