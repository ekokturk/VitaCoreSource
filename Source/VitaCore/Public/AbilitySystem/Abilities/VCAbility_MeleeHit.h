// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameTypes.h"
#include "VCGameplayAbility.h"
#include "VCAbility_MeleeHit.generated.h"

class UVCMeleeComponent;
class UGameplayEffect;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayTagAdded;
class UAbilityTask_WaitGameplayTagRemoved;

/*
 *	This ability uses VCMeleeComponent that actor has to collide with targets while applying an effect
 *	Collision can be activated/deactivated based on a timer or when a gameplay tag is applied to the owner
 *	On collision a pushback force can be applied to the target
 */

USTRUCT()
struct FVCAbility_MeleeHitData
{
	GENERATED_BODY()

	FVCAbility_MeleeHitData()
		:ComponentTag(NAME_None),
		Animation(nullptr),
		MontageSectionName(NAME_None),
		MeleeComponentRef(nullptr){};

	UPROPERTY(EditAnywhere)
		FName ComponentTag;

	UPROPERTY(EditAnywhere)
		UAnimMontage* Animation;

	UPROPERTY(EditAnywhere)
		FName MontageSectionName;

	UPROPERTY(Transient)
		UVCMeleeComponent* MeleeComponentRef;

	void SetMeleeComponentRef(UVCMeleeComponent* MeleeComponent) { MeleeComponentRef = MeleeComponent; }

	bool IsValid() const { return MeleeComponentRef != nullptr && Animation != nullptr; }
};


UCLASS()
class VITACORE_API UVCAbility_MeleeHit : public UVCGameplayAbility
{
	GENERATED_BODY()
	
public:
	UVCAbility_MeleeHit();
	
protected:

	UPROPERTY(EditAnywhere, Category = "Melee Hit Ability")
		TArray< FVCAbility_MeleeHitData> MeleeComponentInfo;
	
	// Gameplay Effect to be applied with melee hit
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability")
		TSubclassOf<UGameplayEffect> MeleeHitEffect;

	UPROPERTY()
		FGameplayEffectSpecHandle MeleeHitEffectHandle;

	// ------- MELEE PUSHBACK ------
	// Amount of force that will be applied for pushback
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability")
		float HitForceMagnitude;

	// If this is true, force will be applied from source to target, otherwise it will be applied from forward vector of source
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability")
		bool IsMeleePushbackRelative;

	// It will override Z of the normalized direction vector (If it is negative it wont change Z direction)
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability")
		float VerticalPushbackOverride;
	
	// Tags to prevent pushback for the character
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability")
		FGameplayTagContainer TagsToPreventMeleePushback;
	
	// ------- TIMER MELEE ACTIVATION -------
	// Initial delay to start melee collision after ability started
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability", meta = (EditCondition = "!bShouldActivateMeleeWithEventTag"))
		float MeleeActivationInitalDelay;
	
	// Duration of the melee collision after initial delay ends
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability", meta = (EditCondition = "!bShouldActivateMeleeWithEventTag"))
		float MeleeActivationDuration;

	UPROPERTY()
		FTimerHandle TimerHandle_MeleeActivationInitialDelay;

	UPROPERTY()
		FTimerHandle TimerHandle_MeleeActivationDuration;

	// ------- GAMEPLAY EVENT MELEE ACTIVATION -------
	// If this is true, melee collision activation will be based on a event instead of a timer
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability", meta = (InlineEditConditionToggle))
		bool bShouldActivateMeleeWithEventTag;

	// Tag for melee collision activation
	UPROPERTY(EditDefaultsOnly, Category = "Melee Hit Ability", meta = (EditCondition = "bShouldActivateMeleeWithEventTag"))
		FGameplayTag MeleeActiveTag;

	// ------- TASKS -------
	UPROPERTY()
		UAbilityTask_PlayMontageAndWait* Task_PlayMontageAndWaitStart;

	UPROPERTY()
		UAbilityTask_WaitGameplayTagAdded* Task_WaitMeleeTagAdded;
	
	UPROPERTY()
		UAbilityTask_WaitGameplayTagRemoved* Task_WaitMeleeTagRemoved;

	//UPROPERTY()
	//	TArray<UVCMeleeComponent*> OwnerMeleeComponents;

	// Callback for Melee Hit is triggered from collision
	UFUNCTION()
		virtual void OnMeleeHit(const FHitInfo HitInfo);

	// Callback for the start of melee attack
	UFUNCTION()
		virtual void OnMeleeStart();

	// Callback for the end of melee attack
	UFUNCTION()
		virtual void OnMeleeEnd();
	
	// Gets the vector for the melee pushback
	UFUNCTION()
		FVector GetMeleeHitImpulseVector(const FTransform& TargetTransform) const;

	UPROPERTY(Transient)
		FVCAbility_MeleeHitData CurrentMeleeInfo;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
