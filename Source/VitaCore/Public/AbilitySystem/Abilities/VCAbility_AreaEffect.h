// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameTypes.h"
#include "VCAbility_PlayAnimation.h"
#include "VCAbility_AreaEffect.generated.h"

class AVCAreaVolume;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/*
 * This ability spawns an volumetric area that has a gameplay effect
 * Spawn action can be done either with a Gameplay Event or When ability ends
 * Area Volume can be spawned with an offset in a circular pattern or directly at the location of the owner
 */

UENUM()
enum class EVCAbility_AreaEffect_TargetTypes : uint8
{
	Self = 0,
	AllEnemyCharacters,
	AllAllyCharacters
};

UCLASS()
class VITACORE_API UVCAbility_AreaEffect : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()

public:
	UVCAbility_AreaEffect();

protected:
	// ------- EFFECT AREA -------------
	// Gameplay effect area that will be spawned at the end of ability or with a gameplay tag
		UPROPERTY(EditAnywhere, Category = "Area Effect Ability")
		TSubclassOf<AVCAreaVolume> EffectArea;
	
	// Who will area effect targets
	UPROPERTY(EditAnywhere, Category = "Area Effect Ability")
		EVCAbility_AreaEffect_TargetTypes TargetType;
	
	// Spawn when gameplay tag is applied instead of spawning when ability is over
	UPROPERTY(EditAnywhere, Category = "Area Effect Ability", meta = (InlineEditConditionToggle))
		bool bShouldSpawnWithTag;

	// Gameplay tag expected by the task to spawn effect area
	UPROPERTY(EditAnywhere, Category = "Area Effect Ability", meta = (EditCondition = "bShouldSpawnWithTag"))
		FGameplayTag EventTagToSpawn;

	// If true area effect should spawn using Spawn around parameters
	UPROPERTY(EditAnywhere, Category = "Area Effect Ability", meta = (InlineEditConditionToggle))
		bool bShouldSpawnAround;

	UPROPERTY(EditAnywhere, Category = "Area Effect Ability", meta = (EditCondition = "bShouldSpawnAround"))
		uint32 Count;
	
	// Parameters to spawn area effect around the target (Circular pattern)
	UPROPERTY(EditAnywhere, Category = "Area Effect Ability", meta = (EditCondition = "bShouldSpawnAround"))
		FSpawnAroundParams SpawnAroundParams;

	// Gameplay Effect area that will be spawned
	UFUNCTION()
		void SpawnEffectArea(FGameplayEventData Payload = FGameplayEventData());

	// Get Target actors to spawn area effect on (Determined based on Target Type)
	UFUNCTION()
		TArray<AActor*> GetTargetActors() const;
	
	// -------- TASKS -------------
	UPROPERTY()
		UAbilityTask_WaitGameplayEvent* Task_WaitGameplayEvent;

protected:
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
