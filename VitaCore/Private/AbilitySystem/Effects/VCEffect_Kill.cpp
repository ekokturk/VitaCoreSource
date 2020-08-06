// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEffect_Kill.h"

#include "VCHealthAttributeSet.h"
#include "VCEffect_StatusDead.h"

UVCEffect_Kill::UVCEffect_Kill()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// ============== Apply Damage to Kill the Target ==============
	FGameplayModifierInfo ModifierInfo;
	// Attribute to apply effect to
	ModifierInfo.Attribute = UVCHealthAttributeSet::GetHealthAttribute();
	// Application type
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	// How the effect will be applied
	FAttributeBasedFloat AttributeBasedFloat;
	AttributeBasedFloat.Coefficient = -1.0f;
	AttributeBasedFloat.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(UVCHealthAttributeSet::GetMaxHealthAttribute(),
		EGameplayEffectAttributeCaptureSource::Target, false);
	FGameplayEffectModifierMagnitude(FAttributeBasedFloat(AttributeBasedFloat));
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FAttributeBasedFloat(AttributeBasedFloat));
	Modifiers.Add(ModifierInfo);

	// !!! Attribute set should handle this !!!
	// ============== Apply Dead Status ==============
	//FConditionalGameplayEffect ConditionalGameplayEffect;
	//ConditionalGameplayEffect.EffectClass = UVCEffect_StatusDead::StaticClass();
	//ConditionalGameplayEffects.Add(ConditionalGameplayEffect);

}
