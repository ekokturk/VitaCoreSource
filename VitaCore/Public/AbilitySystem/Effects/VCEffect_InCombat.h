// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "VCEffect_InCombat.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCEffect_InCombat : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UVCEffect_InCombat();

private:
	UPROPERTY()
		float InCombatDuration;
};
