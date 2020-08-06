// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEffect_InCombat.h"

UVCEffect_InCombat::UVCEffect_InCombat()
	:InCombatDuration(5.0f)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(InCombatDuration));
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Status.InCombat"));
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;
}
