// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCHealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "VCCharacter.h"
#include "VCCoreGameState.h"
#include "VCEffect_InCombat.h"

UVCHealthAttributeSet::UVCHealthAttributeSet()
	:MaxHealth(100.0f),
	 HealthRegen(0.0f)
{
	Health = MaxHealth;

	Tag_FullHealth = FGameplayTag::RequestGameplayTag(FName("Attributes.Health.Full"));
}

bool UVCHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if((Data.EvaluatedData.Attribute.GetUProperty() == GetHealthAttribute().GetUProperty()) &&
		Data.EvaluatedData.Magnitude < 0 && OnModifyDamage.IsBound())
	{
		UAbilitySystemComponent* SourceASC = Data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent();
		if(SourceASC)
		{
			OnModifyDamage.Execute(SourceASC->GetOwner(), Data.EvaluatedData.Magnitude);
		}
	}
	
	return true;
}

void UVCHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UVCHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	// ------------------------ HEALTH CHANGE ------------------------

	if (Data.EvaluatedData.Attribute.GetUProperty() == GetHealthAttribute().GetUProperty())
	{
		// Clamp Health
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
		
		IVCEntityInterface* OwnerEntity = Cast<IVCEntityInterface>(GetOuter());
		if (OwnerEntity != nullptr && Source != nullptr)
		{
			if(Data.EvaluatedData.Magnitude < 0)
			{
				// Notify Damage
				OnDamage.Broadcast(Source->GetAvatarActor(), -Data.EvaluatedData.Magnitude, Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

				// Apply InCombat effect
				const FGameplayEffectSpecHandle SpecHandle = GetOwningAbilitySystemComponent()->MakeOutgoingSpec(UVCEffect_InCombat::StaticClass(),
					0, FGameplayEffectContextHandle());
				GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			// Notify Death
			if (Health.GetCurrentValue() <= 0 && OwnerEntity->IsAlive() == true )
			{
				OwnerEntity->DoDeath(FDeathInfo(Source->GetAvatarActor()));
			}
		}

		
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetMaxHealthAttribute().GetUProperty()) {}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetHealthRegenAttribute().GetUProperty()) {}
}


// ============================ REPLICATION NOTIFY START =======================================

void UVCHealthAttributeSet::OnRep_Health(FGameplayAttributeData PrevHealth)
{
	const float PreviousHealth = PrevHealth.GetCurrentValue();
	const float CurrentHealth = Health.GetCurrentValue();
	if(PreviousHealth > CurrentHealth)
	{
		OnDamage.Broadcast(nullptr, PreviousHealth - CurrentHealth, CurrentHealth, MaxHealth.GetCurrentValue());
	}
	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCHealthAttributeSet, Health, PrevHealth);
}

void UVCHealthAttributeSet::OnRep_MaxHealth(FGameplayAttributeData PrevMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCHealthAttributeSet, MaxHealth, PrevMaxHealth);
}

void UVCHealthAttributeSet::OnRep_HealthRegen(FGameplayAttributeData PrevHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCHealthAttributeSet, HealthRegen, PrevHealthRegen);
}

void UVCHealthAttributeSet::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UVCHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCHealthAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
}