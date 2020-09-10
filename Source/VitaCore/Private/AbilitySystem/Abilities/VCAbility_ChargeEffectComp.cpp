// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_ChargeEffectComp.h"
#include "VCEffectComponent.h"

UVCAbility_ChargeEffectComp::UVCAbility_ChargeEffectComp()
{
}

void UVCAbility_ChargeEffectComp::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ActorEffectComponents = GetAbilityActorComponents<UVCEffectComponent>(ActorInfo, EffectComponentTag);

}

void UVCAbility_ChargeEffectComp::DoOnChargeStarted()
{
	UVCAbility_Charge::DoOnChargeStarted();
	for(auto EffectComponent : ActorEffectComponents)
	{
		EffectComponent->SetOverlapActive(true);
		if(bShouldUseMultipleEffectComponent)
		{
			break;
		}
	}
}

void UVCAbility_ChargeEffectComp::DoOnChargeEnded(float ChargeDuration)
{
	UVCAbility_Charge::DoOnChargeEnded(ChargeDuration);
	for (auto EffectComponent : ActorEffectComponents)
	{
		EffectComponent->SetOverlapActive(false);
		if (bShouldUseMultipleEffectComponent)
		{
			break;
		}
	}
}
