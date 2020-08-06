// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_PlayAnimation.h"
#include "VCAbility_Spawn.generated.h"

class UVCHealthAttributeSet;

UCLASS()
class VITACORE_API UVCAbility_Spawn : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()
public:
	UVCAbility_Spawn();
	
protected:
	UPROPERTY()
		UVCHealthAttributeSet* HealthAttributeSet;

	UPROPERTY()
		FGameplayTag Tag_StatusDead;
	
protected:
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
