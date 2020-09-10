// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayAbility.h"
#include "VCAbility_Charge.generated.h"

class UCameraShake;
class UAbilityTask_WaitGameplayTagAdded;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitInputRelease;
class UVCEnergyAttributeSet;

/*
 * This ability is a base for user charged abilities
 * Charge operation uses energy
 */

UCLASS()
class VITACORE_API UVCAbility_Charge : public UVCGameplayAbility
{
	GENERATED_BODY()

public:
	
	UVCAbility_Charge();

protected:

	// -------- EFFECT -------------
	// Gameplay effect that will be applied during chargin
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		TSubclassOf<UGameplayEffect> ChargingEffect;
	
	// Handle for gameplay effect that will be applied during charging
	UPROPERTY()
		FActiveGameplayEffectHandle ChargingEffectHandle;

	// When there is no energy charge is over
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		bool bShouldEndWhenEnergyIsOut;
	
	// Tags that will be applied during charging (Does not replicate)
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		FGameplayTagContainer TagContainer_Charging;

	// Gameplay Event tag that will stop charging for Non-Player owners ()
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		FGameplayTag Tag_ChargeRelease;
	
	// Gameplay cue tag that will be applied for the duration of charging
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		FGameplayTag GameplayCue_Charging;

	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		TSubclassOf<UCameraShake> ChargeCameraShake;
	
	// Energy at the begining of ability
	UPROPERTY()
		float InitialEnergy;

	// Energy when charging is over
	UPROPERTY()
		float ReleaseEnergy;

	// Energy when charging is over
	UPROPERTY()
		float CostEnergy;

	// Reference cache for attribute system for energy attribute
	UPROPERTY()
		UVCEnergyAttributeSet* EnergyAttributeSet;

	
	// -------- ANIMATION -------------
	// Animation plays when charging begins (Stops when ability is over)
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		UAnimMontage* StartAnimation;

	// ANimation play when charging ends
	UPROPERTY(EditAnywhere, Category = "Charge Ability")
		UAnimMontage* EndAnimation;
	
	// -------- TASKS -------------
	
	UPROPERTY()
		UAbilityTask_PlayMontageAndWait* Task_PlayMontageAndWaitStart;

	UPROPERTY()
		UAbilityTask_PlayMontageAndWait* Task_PlayMontageAndWaitEnd;

	UPROPERTY()
		UAbilityTask_WaitInputRelease* Task_WaitInputRelease;

	UPROPERTY()
		UAbilityTask_WaitGameplayTagAdded* Task_WaitReleaseGameplayTag;

protected:
	
	UFUNCTION()
	// Callback for charge ended without duration count (Calls version with duration argument with negative duration)
		void OnChargeEnded_NoDuration();
	
	UFUNCTION()
	// Callback function when the charge is over 
		void OnChargeEnded(float ChargeDuration);

	// Function to be overridden for additional functionality when the charge starts
	UFUNCTION()
		virtual void DoOnChargeStarted();

	// Function to be overridden for additional functionality when the charge ends
	UFUNCTION()
		virtual void DoOnChargeEnded(float ChargeDuration);

	UFUNCTION()
		float GetUsedEnergy() const { return (InitialEnergy - ReleaseEnergy); }

	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

