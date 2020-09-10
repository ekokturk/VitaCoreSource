// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEffect_FullHeal.h"

#include "VCHealthAttributeSet.h"

UVCEffect_FullHeal::UVCEffect_FullHeal()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// ============== Replenish Full Health ==============
	FGameplayModifierInfo ModifierInfo;
	// Attribute to apply effect to
	ModifierInfo.Attribute = UVCHealthAttributeSet::GetHealthAttribute();
	// Application type
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	// How the effect will be applied
	FAttributeBasedFloat AttributeBasedFloat;
	AttributeBasedFloat.Coefficient = 1.0f;
	AttributeBasedFloat.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(UVCHealthAttributeSet::GetMaxHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target, false);
	FGameplayEffectModifierMagnitude(FAttributeBasedFloat(AttributeBasedFloat));
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FAttributeBasedFloat(AttributeBasedFloat));
	Modifiers.Add(ModifierInfo);
}
