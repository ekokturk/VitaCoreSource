// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayAbility.h"
#include "VCAbility_PlayAnimation.generated.h"

class UAbilityTask_PlayMontageAndWait;

/*
 * Ability that plays an animation montage
 * This is a base class for single animation abilities that end when animation is over
 */

UCLASS()
class VITACORE_API UVCAbility_PlayAnimation : public UVCGameplayAbility
{
	GENERATED_BODY()

public:
	UVCAbility_PlayAnimation();

protected:
	// Animation montage to be played
	UPROPERTY(EditDefaultsOnly, Category = "Animation Ability")
		UAnimMontage* AbilityAnimation;

	// Section to start animation from
	UPROPERTY(EditDefaultsOnly, Category = "Animation Ability")
		FName AnimationStartSection;

	// If true stop the animation when ability is over
	UPROPERTY(EditDefaultsOnly, Category = "Animation Ability")
		bool bShouldAnimationStopWhenAbilityEnds;

	// If true end ability when the animation is over
	UPROPERTY(EditDefaultsOnly, Category = "Animation Ability")
		bool bShouldEndAbilityWithAnimation;

	// ------- TASKS ---------
	UPROPERTY()
		UAbilityTask_PlayMontageAndWait* Task_PlayMontageAndWaitStart;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION()
		virtual void OnAnimationEnded();
};
