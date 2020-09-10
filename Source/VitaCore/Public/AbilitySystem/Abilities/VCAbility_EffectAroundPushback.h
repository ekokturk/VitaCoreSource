// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_EffectAroundSelf.h"
#include "VCAbility_EffectAroundPushback.generated.h"

/*
 *	This ability will apply an effect to actors around the ability owner with an impulse force
 *	Effect can be applied when a gameplay event is applied to the owner, when ability begins or ends
 */

UCLASS()
class VITACORE_API UVCAbility_EffectAroundPushback : public UVCAbility_EffectAroundSelf
{
	GENERATED_BODY()
public:
	UVCAbility_EffectAroundPushback();
	
protected:
	
	// ------- PUSHBACK ------
	// Amount of force that will be applied from source to target on horizontal plane
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Pushback Ability")
		float RelativePushbackMagnitude;
	
	// Amount of force that will be applied from source to target on horizontal plane
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Pushback Ability")
		float VerticalPushbackMagnitude;
	
	// Tags to prevent pushback for the character
	UPROPERTY(EditDefaultsOnly, Category = "Effect Around Pushback Ability")
		FGameplayTagContainer TagsToPreventPushback;
	
protected:
	virtual void OnEffectApplied(AActor* EffectedActor) override;

};
