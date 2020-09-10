// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_Death.h"


#include "AbilitySystemComponent.h"
#include "VCHealthAttributeSet.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "VCEffect_StatusDead.h"
#include "VCEffect_Kill.h"
#include "VCPlayerController.h"
#include "VCHUD_Player.h"


UVCAbility_Death::UVCAbility_Death()
{
	bShouldEndAbilityWithAnimation = true;
	bShouldAnimationStopWhenAbilityEnds = false;
}

void UVCAbility_Death::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	if(HasAuthority(&GetCurrentActivationInfoRef()))
	{
		UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
		if(HealthAttributeSet)
		{
			// Kill Actor if it is still alive
			if(HealthAttributeSet->GetHealth() > 0)
			{
				const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UVCEffect_Kill::StaticClass(),
					GetOwnerAbilityLevel(), FGameplayEffectContextHandle());
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

			// Apply Dead Status with a Tag
			const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UVCEffect_StatusDead::StaticClass(),
				GetOwnerAbilityLevel(), FGameplayEffectContextHandle());
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void UVCAbility_Death::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// Cache base attribute set
	for (auto SpawnedAttributeSet : GetAbilitySystemComponentFromActorInfo()->SpawnedAttributes)
	{
		if(UVCHealthAttributeSet * AttributeSet = Cast<UVCHealthAttributeSet>(SpawnedAttributeSet))
		{
			HealthAttributeSet = AttributeSet;
			break;
		}
	}
}
