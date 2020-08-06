// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCEnergyAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "VCHeroCharacter.h"


UVCEnergyAttributeSet::UVCEnergyAttributeSet()
	:MaxEnergy(150.0f),
	 EnergyRegen(1.0f)
{
	Energy = MaxEnergy;

	Tag_FullEnergy = FGameplayTag::RequestGameplayTag(FName("Attributes.Energy.Full"));

}


void UVCEnergyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	// ENERGY CHANGE
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UVCEnergyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UVCEnergyAttributeSet, Energy)))
	{
		// Clamp Energy
		Energy.SetCurrentValue(FMath::Clamp(Energy.GetCurrentValue(), 0.0f, MaxEnergy.GetCurrentValue()));
		Energy.SetBaseValue(FMath::Clamp(Energy.GetBaseValue(), 0.0f, MaxEnergy.GetBaseValue()));
		AVCHeroCharacter * OwningPlayerCharacter = Cast<AVCHeroCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());

		if(OwningPlayerCharacter != nullptr)
		{
			//if (MaxEnergy.GetCurrentValue() == Energy.GetCurrentValue())
			//{
			//	OwningPlayerCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(Tag_FullEnergy);
			//}
			//else
			//{
			//	OwningPlayerCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag_FullEnergy);
			//}
		}

	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetMaxEnergyAttribute().GetUProperty()) {}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetEnergyRegenAttribute().GetUProperty()) {}

}


// ============================ REPLICATION NOTIFY START =======================================
// ENERGY
void UVCEnergyAttributeSet::OnRep_Energy(FGameplayAttributeData PrevEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCEnergyAttributeSet, Energy, PrevEnergy);
}
// MAX ENERGY
void UVCEnergyAttributeSet::OnRep_MaxEnergy(FGameplayAttributeData PrevMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCEnergyAttributeSet, MaxEnergy, PrevMaxEnergy);
}

// ENERGY REGEN
void UVCEnergyAttributeSet::OnRep_EnergyRegen(FGameplayAttributeData PrevEnergyRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCEnergyAttributeSet, EnergyRegen, PrevEnergyRegen);
}
// ============================ REPLICATION NOTIFY END =======================================

// Replicate properties
void UVCEnergyAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UVCEnergyAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCEnergyAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCEnergyAttributeSet, EnergyRegen, COND_None, REPNOTIFY_Always);

}