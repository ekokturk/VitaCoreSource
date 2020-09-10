// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_PlayAnimation.h"
#include "VCAbility_Death.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UVCHealthAttributeSet;

UCLASS()
class VITACORE_API UVCAbility_Death : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()
public:
	UVCAbility_Death();

protected:
	UPROPERTY() UVCHealthAttributeSet* HealthAttributeSet;
	
protected:
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;


	
};
