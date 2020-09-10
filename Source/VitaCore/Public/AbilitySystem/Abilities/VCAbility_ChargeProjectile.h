// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_Charge.h"
#include "VCShooterComponentTypes.h"

#include "VCAbility_ChargeProjectile.generated.h"

class AVCProjectile;
class UVCShooterComponent;
/*
 * This ability allows player to charge to shoot a projectile
 * It detects the consumed energy to amplify projectile Speed, Damage and Count
 * Consumed energy is converted into steps based on cost or custom value to calculate adjusted count
 */

UCLASS()
class VITACORE_API UVCAbility_ChargeProjectile : public UVCAbility_Charge
{
	GENERATED_BODY()

public:
	UVCAbility_ChargeProjectile();

protected:
	// ----- CHARGED COUNT ---------
	// Enable this to override amount of projectiles to be spawned based on energy
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (InlineEditConditionToggle))
		bool bShouldOverrideProjectileCountWithEnergy;

	// Value to divide used energy into steps to override count (If this is zero and lower use ability cost as the step)
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldOverrideProjectileCountWithEnergy"))
		int ProjectileCountOverrideEnergyStep;

	// Add extra projectiles to each spawn operation [(EnergyUsed*Multiplier/Step) + Offset]
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldOverrideProjectileCountWithEnergy"))
		int ProjectileCountOverrideOffset;

	// Multiply projectiles determined by the step with this value [(EnergyUsed*Multiplier/Step) + Offset]
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldOverrideProjectileCountWithEnergy"))
		int ProjectileCountOverrideMultiplier;

	// ----- CHARGED SPEED ---------
	// Enable this to override speed of the projectile
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (InlineEditConditionToggle))
		bool bShouldIncreaseSpeedWithEnergy;

	// Multiply energy used with this parameter and add it to base state to increase speed in game units
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldIncreaseSpeedWithEnergy"))
		float ProjectileSpeedEnergyBoostMultiplier;

	// Cache default speed before overriding
	UPROPERTY()
		float DefaultInitialSpeed;
	
	// ----- CHARGED DAMAGE ---------
	// Enable this to increase damage of the projectile based on energy used
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (InlineEditConditionToggle))
		bool bShouldIncreaseDamageWithEnergy;

	// Tag to use to increase damage for effect calculations
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldIncreaseDamageWithEnergy"))
		FGameplayTag EnergyDamageBoostDataTag;

	// Multiply damage with this magnitude
	UPROPERTY(EditAnywhere, Category = "Charged Projectile Ability", meta = (EditCondition = "bShouldIncreaseDamageWithEnergy"))
		float EnergyDamageBoostMagnitude;
	
	// -------- PROJECTILE ------------
	// Effect to be applied to the projectile
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
	TSubclassOf<UGameplayEffect> ProjectileEffect;

	// Projectile object to be spawned
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		TSubclassOf<AVCProjectile> Projectile;

	// Shooting parameters for shooter component
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		FProjectileShootParams ProjectileParams;

	// -------- SHOOTER COMPONENT ------------
	// Get specific shooter component that has this tag otherwise get all shooter components
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		FName ShooterComponentTag;

	// Shoot from all available shooter components with the chosen tag
	UPROPERTY(EditAnywhere, Category = "Projectile Ability")
		bool ShouldUseMultipleShooterComponent;

	// Called whenever a projectile is spawned
	UFUNCTION()
		virtual void OnShooterProjectileSpawned(AVCProjectile * SpawnedProjectile);
	
	UPROPERTY()
		TArray<UVCShooterComponent*> ActorShooterComponents;
	
protected:
		virtual void PostLoad() override;
		virtual void DoOnChargeEnded(float ChargeDuration) override;
		virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
		virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
