// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_Charge.h"
#include "VCEntityInterface.h"
#include "Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tasks/AbilityTask_WaitInputRelease.h"
#include "VCEnergyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask_WaitGameplayTag.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"


UVCAbility_Charge::UVCAbility_Charge()
	:bShouldEndWhenEnergyIsOut(false)
{
}



void UVCAbility_Charge::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// Cache energy attribute set
	for (UAttributeSet* Set : GetAbilitySystemComponentFromActorInfo()->SpawnedAttributes)
	{
		if (UVCEnergyAttributeSet* PlayerSet = Cast<UVCEnergyAttributeSet>(Set))
		{
			EnergyAttributeSet = PlayerSet;
		}
	}
}


void UVCAbility_Charge::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CostEffect != nullptr)
	{
		if (CostEffect->Modifiers.Num() > 0) // If there are effects
		{
			const FGameplayModifierInfo EffectInfo = CostEffect->Modifiers[0];
			EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetOwnerAbilityLevel(), CostEnergy);
		}
	}

	// Initialize Energy Attribute
	if (EnergyAttributeSet != nullptr)
	{
		InitialEnergy = EnergyAttributeSet->Energy.GetCurrentValue() + FMath::Abs(CostEnergy);
	}
	
	// Cost Energy Attribute
	if (EnergyAttributeSet != nullptr)
	{
		CostEnergy = InitialEnergy - EnergyAttributeSet->Energy.GetCurrentValue();
	}
	if(GameplayCue_Charging.IsValid())
	{
		K2_AddGameplayCue(GameplayCue_Charging, FGameplayEffectContextHandle());
	}
	
	// Play start animation
	if (StartAnimation != nullptr && GetActorInfo().GetAnimInstance() != nullptr)
	{
		Task_PlayMontageAndWaitStart = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
			FName(TEXT("None")), StartAnimation, 1.0f, FName(TEXT("None")), true);
		Task_PlayMontageAndWaitStart->ReadyForActivation();
	}

	// Wait for input release if the actor is player controlled
	if(ActorInfo->PlayerController != nullptr)
	{
		if (ChargeCameraShake)
		{
			ActorInfo->PlayerController->ClientPlayCameraShake(ChargeCameraShake);
		}
		
		Task_WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
		Task_WaitInputRelease->OnRelease.AddDynamic(this, &UVCAbility_Charge::OnChargeEnded);
		Task_WaitInputRelease->ReadyForActivation();
	}
	else
	{
		Task_WaitReleaseGameplayTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, Tag_ChargeRelease, nullptr, true);
		Task_WaitReleaseGameplayTag->Added.AddDynamic(this, &UVCAbility_Charge::OnChargeEnded_NoDuration);
		Task_WaitReleaseGameplayTag->ReadyForActivation();
	}

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTags(TagContainer_Charging);
	
	// Apply charging effect
	if(ChargingEffect != nullptr)
	{
		ChargingEffectHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, ChargingEffect.GetDefaultObject(), GetOwnerAbilityLevel());
	}

	if(bShouldEndWhenEnergyIsOut)
	{
		GetAbilitySystemComponentFromActorInfo()->
		GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetEnergyAttribute())
		.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData)
		{
			if(AttributeChangeData.NewValue <= 0)
			{
				Task_WaitInputRelease->EndTask();
				OnChargeEnded(0.0f);
			}
		});
	}


	
	DoOnChargeStarted();
}

void UVCAbility_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTags(TagContainer_Charging);

	if (ChargeCameraShake && ActorInfo && ActorInfo->PlayerController.IsValid())
	{
		ActorInfo->PlayerController->ClientStopCameraShake(ChargeCameraShake);
	}
	
	BP_RemoveGameplayEffectFromOwnerWithHandle(ChargingEffectHandle);
	if(bShouldEndWhenEnergyIsOut)
	{
		GetAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetEnergyAttribute()).RemoveAll(this);
	}
}


void UVCAbility_Charge::OnChargeEnded_NoDuration()
{
	OnChargeEnded(-1.f);
}

void UVCAbility_Charge::OnChargeEnded(float ChargeDuration)
{
	if (EnergyAttributeSet != nullptr)
	{
		ReleaseEnergy = EnergyAttributeSet->Energy.GetCurrentValue();
	}
	
	if (EndAnimation != nullptr && GetActorInfo().GetAnimInstance() != nullptr)
	{
		Task_PlayMontageAndWaitEnd = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
			FName(TEXT("None")), EndAnimation, 1.0f, FName(TEXT("None")), false);
		Task_PlayMontageAndWaitEnd->ReadyForActivation();
	}
	
	DoOnChargeEnded(ChargeDuration);
}

void UVCAbility_Charge::DoOnChargeStarted()
{
	
}

void UVCAbility_Charge::DoOnChargeEnded(float ChargeDuration)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
