// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_Spawn.h"

#include "VCHealthAttributeSet.h"
#include "VCEffect_StatusDead.h"
#include "VCEffect_FullHeal.h"
#include "VCPlayerController.h"

UVCAbility_Spawn::UVCAbility_Spawn()
{
	Tag_StatusDead = FGameplayTag::RequestGameplayTag("Status.Dead");
}

void UVCAbility_Spawn::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	AVCPlayerController* VCPlayerController = Cast<AVCPlayerController>(ActorInfo->PlayerController);
	if(VCPlayerController)
	{
	}
	
	if (HasAuthority(&GetCurrentActivationInfoRef()))
	{
		// Remove Dead Status 
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(Tag_StatusDead));

		// Fully Heal
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UVCEffect_FullHeal::StaticClass(),
			GetOwnerAbilityLevel(), FGameplayEffectContextHandle());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UVCAbility_Spawn::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// Cache base attribute set
	for (auto SpawnedAttributeSet : GetAbilitySystemComponentFromActorInfo()->SpawnedAttributes)
	{
		if (UVCHealthAttributeSet* AttributeSet = Cast<UVCHealthAttributeSet>(SpawnedAttributeSet))
		{
			HealthAttributeSet = AttributeSet;
			break;
		}
	}
}
