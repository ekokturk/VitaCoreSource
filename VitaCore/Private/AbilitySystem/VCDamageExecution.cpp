// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCDamageExecution.h"
#include "VCHealthAttributeSet.h"
#include "VCTraitAttributeSet.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "VCCoreGameMode.h"
#include "VCVitaCoreShrine.h"
#include "Components/CapsuleComponent.h"

struct VCDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Mastery);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Fortitude);

	VCDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVCTraitAttributeSet, Mastery, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVCTraitAttributeSet, Fortitude, Target, true);
	}
};

static const VCDamageStatics& DamageStatics()
{
	static VCDamageStatics DmgStatics;
	return DmgStatics;
}

UVCDamageExecution::UVCDamageExecution()
{
	HealthProperty = FindFieldChecked<FProperty>(UVCHealthAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UVCHealthAttributeSet, Health));

	RelevantAttributesToCapture.Add(VCDamageStatics().MasteryDef);
	RelevantAttributesToCapture.Add(VCDamageStatics().FortitudeDef);
}

void UVCDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent * TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent * SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();


	if(TargetASC == nullptr || SourceASC == nullptr)
	{
		return;
	}
	
	// Source and Target entity
	IVCEntityInterface* TargetEntity = Cast<IVCEntityInterface>(TargetASC->GetOwner());
	IVCEntityInterface* SourceEntity = Cast<IVCEntityInterface>(SourceASC->GetOwner());

	// Get tage from effect
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters Params;

	float AttackDamageMagnitude = 0.0f;
	// Gets attack damage magnitude
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MasteryDef, FAggregatorEvaluateParameters(), AttackDamageMagnitude);
	float FortitudeMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FortitudeDef, FAggregatorEvaluateParameters(), FortitudeMagnitude);

	const float CalculatedDamage = AttackDamageMagnitude * (1 - FortitudeMagnitude / 200.0f);
	
	// Calculation for damage
	const float FinalDamage = FMath::Clamp(CalculatedDamage, 0.0f, CalculatedDamage);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -FinalDamage));

}
