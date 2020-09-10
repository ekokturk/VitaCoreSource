// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_PlayAnimation.h"
#include "VCAbility_EffectAroundSelf.generated.h"

class UAbilityTask_WaitGameplayEvent;

/*
 *	This ability will apply an effect to actors around the ability owner
 *	Effect can be applied when a gameplay event is applied to the owner, when ability begins or ends
 */

UENUM()
enum class EVCAbility_EffectAroundSelfTriggerState : uint8
{
	OnStart = 0,
	OnEvent,
	OnEnd
};

UCLASS()
class VITACORE_API UVCAbility_EffectAroundSelf : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()
public:
	UVCAbility_EffectAroundSelf();

protected:
	// ------- EFFECT AROUND SELF -------
	// Gameplay Effect to be applied with melee hit
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability")
		TSubclassOf<UGameplayEffect> AroundSelfEffect;
	
	UPROPERTY()
		FGameplayEffectSpecHandle AroundSelfEffectHandle;

	// This will determine the if actor can be effected based on it's affiliation
	UPROPERTY(EditAnywhere, Category = "Effect Around Self Ability", meta = (Bitmask, BitmaskEnum = "EAffiliationType"))
		int EffectAffiliationMask;

	// If true effect will be applied to self besides allied targets
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability", meta = (EditCondition = "bIsFriendlyEffect"))
		bool bShouldApplyEffectToSelf;

	// State of the ability that will trigger the effect
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability")
		EVCAbility_EffectAroundSelfTriggerState EffectTriggerState;

	// Collision channel for actors to get effected
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability", meta = (Bitmask, BitmaskEnum = "ECollisionChannel"))
		int EffectCollisionChannel;
	
	// Tag for melee collision activation
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability", meta = (EditCondition = "EffectTriggerState==EVCAbility_EffectAroundSelfTriggerState::OnEvent"))
		FGameplayTag GameplayEventTag;

	// Radius of the effect
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Self Ability")
		float EffectRadius;

	// ------- TASKS -------
	UPROPERTY()
		UAbilityTask_WaitGameplayEvent* Task_WaitGameplayEvent;

protected:
	// Callback for the start of melee attack
	UFUNCTION()
		void OnEffectTriggered(const FGameplayEventData Payload);

	// Called before a target gets effected
	UFUNCTION()
		virtual void OnEffectApplied(AActor * EffectedActor);

protected:
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	void OnAnimationEnded() override;
};
