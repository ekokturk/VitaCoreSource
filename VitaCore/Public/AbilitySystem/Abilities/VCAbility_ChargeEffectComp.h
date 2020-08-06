// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_Charge.h"
#include "VCAbility_ChargeEffectComp.generated.h"

class UVCEffectComponent;

/**
 * This ability activates effect components that are attached to the target
 * Activation will occur when charging starts and it will deactivate once charging ends/energy is depleted
 */

UCLASS()
class VITACORE_API UVCAbility_ChargeEffectComp : public UVCAbility_Charge
{
	GENERATED_BODY()
public:
	UVCAbility_ChargeEffectComp();

protected:
	// -------- EFFECT COMPONENT ------------
	// Get specific effect component that has this tag otherwise get all effect components
	UPROPERTY(EditAnywhere, Category = "Charge Effect Component Ability")
		FName EffectComponentTag;

	// Activate multiple effect components at once
	UPROPERTY(EditAnywhere, Category = "Charge Effect Component Ability")
		bool bShouldUseMultipleEffectComponent;

	// Cached actor effect components to be used to activate effect
	UPROPERTY()
		TArray<UVCEffectComponent*> ActorEffectComponents;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void DoOnChargeStarted() override;
	virtual void DoOnChargeEnded(float ChargeDuration) override;
};
