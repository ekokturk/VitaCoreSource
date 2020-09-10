// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_AreaEffect.h"
#include "VCAbility_LaunchCharacter.generated.h"

/*
 * This ability spawns a volumetric effect area while moving the owner character
 * It can override character gravity based on a timer
 */

UCLASS()
class VITACORE_API UVCAbility_LaunchCharacter : public UVCAbility_AreaEffect
{
	GENERATED_BODY()

	// ------- LAUNCH CONFIG -------------
	// Launch direction according to he local orientation of the character
	UPROPERTY(EditAnywhere, Category = "Launch Character Ability")
		FVector LaunchDirection;

	// Magnitude of launch force
	UPROPERTY(EditAnywhere, Category = "Launch Character Ability")
		float LaunchForce;

	// ------- GRAVITY CONFIG -------------
	// Only change gravity if this is true
	UPROPERTY(EditAnywhere, Category = "Launch Character Ability", meta = (InlineEditConditionToggle))
		bool bShouldChangeGravity;

	// Cache initial gravity
	UPROPERTY()
		float InitialGravity;

	// Changed gravity during the animation
	UPROPERTY(EditAnywhere, Category = "Launch Character Ability", meta = (EditCondition = "bShouldChangeGravity"))
		float LaunchGravity;

	// Delay to revert back gravity change (if animation is over first this is invalidated)
	UPROPERTY(EditAnywhere, Category = "Launch Character Ability", meta = (EditCondition = "bShouldChangeGravity"))
		float LaunchGravityDuration;

	// Timer handle for gravity change
	FTimerHandle TimerHandle_LaunchGravity;

private:
	UPROPERTY()
		AVCCharacter* OwningCharacter;

	// Update gravity scale of character movement component of the owning character
	UFUNCTION()
		void SetOwnerGravity(float NewGravity);
	
protected:
	virtual void OnAnimationEnded() override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


};

