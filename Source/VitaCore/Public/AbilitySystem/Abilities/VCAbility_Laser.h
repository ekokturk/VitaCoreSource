// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_PlayAnimation.h"
#include "VCShooterComponentTypes.h"

#include "VCAbility_Laser.generated.h"

class AVCLaser;
class UVCShooterComponent;

/**
 *	This ability will use shooter component to spawn a laser actor that applies effect to targets in line sight
 *	Laser can have both ally and enemy effects
 *	Shooter component direction will be set to update to properly adjust look direction
 */

UCLASS()
class VITACORE_API UVCAbility_Laser : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()
public:
	UVCAbility_Laser();

protected:
	// Effect to be applied to the allies on collision
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		TSubclassOf<UGameplayEffect> LaserAllyEffect;

	// Effect to be applied to the enemies on collision
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		TSubclassOf<UGameplayEffect> LaserEnemyEffect;
	
	// Laser actor to be spawned
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		TSubclassOf<AVCLaser> LaserClass;

	// Get specific shooter component that has this tag otherwise get all shooter components
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		FName ShooterComponentTag;

	UPROPERTY()
		FLaserShootParams ShootParams;
	
	// Shoot from all available shooter components with the chosen tag
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		bool ShouldUseMultipleShooterComponent;

	// Override update condition to adjust direction that shooter component is facing
	UPROPERTY(EditAnywhere, Category = "Laser Ability")
		bool bShouldOverrideShooterComponentUpdateCondition;
	
	UPROPERTY()
		TArray<UVCShooterComponent*> ActorShooterComponents;

	// Callback method whenever a laser is spawned to set it's properties
	virtual void OnShooterLaserSpawned(AVCLaser* Laser);
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	
};
