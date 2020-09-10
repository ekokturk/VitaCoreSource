// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEffect_StatusDead.h"

UVCEffect_StatusDead::UVCEffect_StatusDead()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Status.Dead"));
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Status.Dead"));
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;
}

