// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEffectComponent.h"

#include "AbilitySystemComponent.h"


UVCEffectComponent::UVCEffectComponent():
	bRemoveEffectOnOverlapEnds(true),
	bShouldUseOwnerAbilityLevel(true),
	EffectLevel(0)
{
	bShouldOverlapWithOwner = false;
	bShouldRemoveActorOnOverlapEnd = true;
}

int UVCEffectComponent::GetEffectLevel() const
{
	return (OwnerAbilitySystem && bShouldUseOwnerAbilityLevel) ? OwnerAbilitySystem->GetAbilityLevel() : EffectLevel;
}


void UVCEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	// Initialize owner ability system
	if(IVCAbilitySystemInterface * AbilityInterface = Cast<IVCAbilitySystemInterface>(GetOwner()))
	{
		OwnerAbilitySystem.SetInterface(AbilityInterface);
		OwnerAbilitySystem.SetObject(GetOwner());
	}
}


void UVCEffectComponent::HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                      const EAffiliationType OtherAffiliation, const FHitInfo HitInfo)
{
	UVCOverlapComponent::HandleOverlapActorAdded(OtherActor, OtherComp, OtherAffiliation, HitInfo);

	UAbilitySystemComponent* OwnerAbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent();
	IAbilitySystemInterface* OtherAbilitySystem = Cast<IVCAbilitySystemInterface>(OtherActor);
	if(OwnerAbilitySystemComponent && OtherAbilitySystem && OtherAbilitySystem->GetAbilitySystemComponent())
	{
		if (OtherAffiliation & OverlappingAffiliationMask)
		{
				const FGameplayEffectSpecHandle EffectSpecHandle = OwnerAbilitySystemComponent->MakeOutgoingSpec(Effect, 
					GetEffectLevel(), FGameplayEffectContextHandle());
				OwnerAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), 
					OtherAbilitySystem->GetAbilitySystemComponent());

		}
	}
	
}

void UVCEffectComponent::HandleOverlapActorRemoved(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	const EAffiliationType OtherAffiliation)
{
	UVCOverlapComponent::HandleOverlapActorRemoved(OtherActor, OtherComp, OtherAffiliation);
	if(bRemoveEffectOnOverlapEnds)
	{
		UAbilitySystemComponent* OwnerAbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent();
		IAbilitySystemInterface* OtherAbilitySystem = Cast<IVCAbilitySystemInterface>(OtherActor);
		if (OwnerAbilitySystemComponent && OtherAbilitySystem && OtherAbilitySystem->GetAbilitySystemComponent())
		{
			if (OtherAffiliation & OverlappingAffiliationMask)
			{
				OtherAbilitySystem->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(Effect, 
					OwnerAbilitySystemComponent);
			}
		}
	}
}
