// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "VCCharacter.h"

UVCGameplayAbility::UVCGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
}


EAffiliationType UVCGameplayAbility::GetAffiliation(AActor* OtherActor) const
{
	return OwnerEntity ? OwnerEntity->GetAffiliation(OtherActor) : EAffiliationType::NEUTRAL;
}

void UVCGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	// TODO: Remove After Refactoring from blueprints ?
	if (bHasBlueprintActivate)
	{
		K2_ActivateAbility();
	}
	
	// Cache gameplay ability data
	SetCurrentActivationInfo(ActivationInfo);
	SetCurrentActorInfo(Handle, ActorInfo);

	if(CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		OnCommitAbility(Handle, ActorInfo, ActivationInfo);
	}
	else
	{
		GetAbilitySystemComponentFromActorInfo()->NotifyAbilityFailed(Handle, this, FGameplayTagContainer());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UVCGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// Cache ability system interface of owner for the ability
	IVCAbilitySystemInterface * OwnerAbilitySystemInterface = Cast<IVCAbilitySystemInterface>(ActorInfo->OwnerActor.Get());
	if(OwnerAbilitySystemInterface != nullptr)
	{
		OwnerAbilitySystem.SetInterface(OwnerAbilitySystemInterface);
		OwnerAbilitySystem.SetObject(ActorInfo->OwnerActor.Get());
	}
	
	// Cache VC entity of owner for the ability
	IVCEntityInterface* OwnerEntityInterface = Cast<IVCEntityInterface>(ActorInfo->OwnerActor.Get());
	if (OwnerEntityInterface != nullptr)
	{
		OwnerEntity.SetInterface(OwnerEntityInterface);
		OwnerEntity.SetObject(ActorInfo->OwnerActor.Get());
	}
}


void UVCGameplayAbility::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	for(auto const ExecutableCue : ExecutableCuesOnCommit)
	{
		K2_ExecuteGameplayCue(ExecutableCue, GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
	}

	for (auto const ActivatableCue : ActivatableCuesOnCommit)
	{
		K2_AddGameplayCueWithParams(ActivatableCue, GetAbilitySystemComponentFromActorInfo()->MakeEffectContext(), true);
	}
}

AVCCharacter* UVCGameplayAbility::GetOwnerCharacter()
{
	return Cast<AVCCharacter>(GetOwningActorFromActorInfo());
}

int UVCGameplayAbility::GetOwnerAbilityLevel() const
{
	return OwnerAbilitySystem ? OwnerAbilitySystem->GetAbilityLevel() : 0;
}

void UVCGameplayAbility::EndCurrentAbility()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

