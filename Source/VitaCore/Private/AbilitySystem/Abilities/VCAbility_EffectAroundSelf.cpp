// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_EffectAroundSelf.h"

#include "AbilitySystemComponent.h"
#include "Tasks/AbilityTask_WaitGameplayEvent.h"
#include "VCEntityInterface.h"
#include "VCGameLibrary.h"

UVCAbility_EffectAroundSelf::UVCAbility_EffectAroundSelf()
	:bShouldApplyEffectToSelf(false),
	EffectCollisionChannel(ECollisionChannel::ECC_Pawn),
	EffectRadius(500.0f)
{
	EffectAffiliationMask |= EAffiliationType::ENEMY;
}

void UVCAbility_EffectAroundSelf::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	if (HasAuthority(&GetCurrentActivationInfoRef()) == false)
	{
		return;
	}

	if (AroundSelfEffect != nullptr)
	{
		AroundSelfEffectHandle = MakeOutgoingGameplayEffectSpec(AroundSelfEffect, GetOwnerAbilityLevel());
	}

	if(EffectTriggerState == EVCAbility_EffectAroundSelfTriggerState::OnEvent)
	{
		Task_WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayEventTag, nullptr, true);
		Task_WaitGameplayEvent->EventReceived.AddDynamic(this, &UVCAbility_EffectAroundSelf::OnEffectTriggered);
		Task_WaitGameplayEvent->ReadyForActivation();
	}
	else if(EffectTriggerState == EVCAbility_EffectAroundSelfTriggerState::OnStart)
	{
		OnEffectTriggered(FGameplayEventData());
	}
}

void UVCAbility_EffectAroundSelf::OnAnimationEnded()
{
	if(EffectTriggerState == EVCAbility_EffectAroundSelfTriggerState::OnEnd)
	{
		OnEffectTriggered(FGameplayEventData());
	}
	Super::OnAnimationEnded();
}

void UVCAbility_EffectAroundSelf::OnEffectTriggered(const FGameplayEventData Payload)
{
	UAbilitySystemComponent* OwnerAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	TArray<FOverlapResult> Overlaps;
	const FCollisionObjectQueryParams CollisionQueryParams = FCollisionObjectQueryParams(EffectCollisionChannel);
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetCurrentActorInfo()->OwnerActor->GetActorLocation(),
		FQuat::Identity, CollisionQueryParams, FCollisionShape::MakeSphere(EffectRadius)))
	{
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if(bShouldApplyEffectToSelf == false && OverlapResult.Actor.Get() == GetActorInfo().OwnerActor)
			{
				continue;
			}
			
			if (IAbilitySystemInterface * OtherAbilitySystem = Cast<IAbilitySystemInterface>(OverlapResult.Actor.Get()))
			{
				// Check if affiliation matches
				if(GetAffiliation(OverlapResult.Actor.Get()) & EffectAffiliationMask)
				{
					OnEffectApplied(OverlapResult.Actor.Get());
					OwnerAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*AroundSelfEffectHandle.Data.Get(),
						OtherAbilitySystem->GetAbilitySystemComponent());
				}
			}
		}
	}

}

void UVCAbility_EffectAroundSelf::OnEffectApplied(AActor* EffectedActor)
{
}


