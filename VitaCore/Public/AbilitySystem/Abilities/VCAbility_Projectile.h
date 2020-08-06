// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_PlayAnimation.h"
#include "VCShooterComponent.h"
#include "VCAbility_Projectile.generated.h"

class AVCProjectile;
class UVCShooterComponent;
class UAbilityTask_PlayMontageAndWait;

/*
 * This ability uses shooter component to spawn a projectile
 * Projectile parameters can be adjusted depending on the desired intention
 * On multi-shot/timed operations, ability ends when final shot triggers the ShotEnded callback
 */

UCLASS()
class VITACORE_API UVCAbility_Projectile : public UVCAbility_PlayAnimation
{
	GENERATED_BODY()
public:
	UVCAbility_Projectile();
protected:

	// Effect to be applied to the projectile
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		TSubclassOf<UGameplayEffect> ProjectileEffect;

	// Projectile object to be spawned
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		TSubclassOf<AVCProjectile> Projectile;

	// Shooting parameters for shooter component
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		FProjectileShootParams ProjectileParams;

	// Get specific shooter component that has this tag otherwise get all shooter components
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		FName ShooterComponentTag;

	// Shoot from all available shooter components with the chosen tag
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		bool ShouldUseMultipleShooterComponent;

	// This method is called when a projectile is spawned from the shooter
	UFUNCTION()
		virtual void OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile);

	// This will be called whenever shooter component stopped shooting delegate is called
	UFUNCTION()
		virtual void OnShooterStoppedShooting(UVCShooterComponent* ShooterComponent);
	
	UPROPERTY()
		TArray<UVCShooterComponent*> ActorShooterComponents;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

