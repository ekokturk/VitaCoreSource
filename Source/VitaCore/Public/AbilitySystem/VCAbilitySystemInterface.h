// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AbilitySystemInterface.h"
#include "VCAbilitySystemInterface.generated.h"

// Delegate to notify the change in combat state. If first parameter is true actor is in combat, else it is not.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInCombatStateChanged, const bool)

UINTERFACE()
class VITACORE_API UVCAbilitySystemInterface : public UAbilitySystemInterface
{
	GENERATED_UINTERFACE_BODY()
};

class VITACORE_API IVCAbilitySystemInterface : public IAbilitySystemInterface
{
	GENERATED_IINTERFACE_BODY()

	// Get Level that is used by ability system
	virtual int GetAbilityLevel() const = 0;
};