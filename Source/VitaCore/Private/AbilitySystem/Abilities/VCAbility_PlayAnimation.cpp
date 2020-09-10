// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_PlayAnimation.h"
#include "AbilityTask_PlayMontageAndWait.h"

UVCAbility_PlayAnimation::UVCAbility_PlayAnimation()
	:AbilityAnimation(nullptr),
	AnimationStartSection(NAME_None),
	bShouldAnimationStopWhenAbilityEnds(true),
	bShouldEndAbilityWithAnimation(true)
{
	
}

void UVCAbility_PlayAnimation::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UVCAbility_PlayAnimation::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVCAbility_PlayAnimation::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	
	// Play animation
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AbilityAnimation != nullptr && AnimInstance != nullptr)
	{
		// Get a random section if it is not specified
		const int32 SectionCount = AbilityAnimation->CompositeSections.Num();
		const FName SecionName = AnimationStartSection == NAME_None && SectionCount > 0 ?
			AbilityAnimation->CompositeSections[FMath::RandHelper(SectionCount)].SectionName : AnimationStartSection;
		
		Task_PlayMontageAndWaitStart = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
			FName(TEXT("None")), AbilityAnimation, 1.0f, SecionName, bShouldAnimationStopWhenAbilityEnds);
		Task_PlayMontageAndWaitStart->OnInterrupted.AddDynamic(this, &UVCAbility_PlayAnimation::OnAnimationEnded);
		Task_PlayMontageAndWaitStart->OnCancelled.AddDynamic(this, &UVCAbility_PlayAnimation::OnAnimationEnded);
		Task_PlayMontageAndWaitStart->OnBlendOut.AddDynamic(this, &UVCAbility_PlayAnimation::OnAnimationEnded);
		Task_PlayMontageAndWaitStart->OnCompleted.AddDynamic(this, &UVCAbility_PlayAnimation::OnAnimationEnded);
		Task_PlayMontageAndWaitStart->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UVCAbility_PlayAnimation::OnAnimationEnded()
{
	if (bShouldEndAbilityWithAnimation)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
