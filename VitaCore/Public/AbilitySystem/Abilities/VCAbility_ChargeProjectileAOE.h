// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_ChargeProjectile.h"
#include "VCAbility_ChargeProjectileAOE.generated.h"

/*
 * This ability allows player to charge to shoot a projectile which deals AOE splash damage on collision
 * It detects the consumed energy to amplify projectile splash damage
 * Effect radius and how much it will decay based on distance can affect applied damage
 */

UCLASS()
class VITACORE_API UVCAbility_ChargeProjectileAOE : public UVCAbility_ChargeProjectile
{
	GENERATED_BODY()
public:
	UVCAbility_ChargeProjectileAOE();

protected:

	// Gameplay Effect to be applied to targets in radius when projectile dies
	UPROPERTY(EditDefaultsOnly, Category = "AOE Charge Projectile Ability")
		TSubclassOf<UGameplayEffect> SplashEffect;
	
	// Radius of the effect
	UPROPERTY(EditDefaultsOnly, Category = "AOE Charge Projectile Ability")
		float EffectRadius;

	// How much effect will decay based on where the target is in radius (Can be from 0 to 1)
	UPROPERTY(EditDefaultsOnly, Category = "AOE Charge Projectile Ability")
		float EffectMagnitudeAtSplashBounds;

	// Multiply damage of the splash effect
	UPROPERTY(EditDefaultsOnly, Category = "AOE Charge Projectile Ability")
		float EnergySplashDamageBoostMagnitude;

	// Collision channel for actors to get effected
	UPROPERTY(EditDefaultsOnly, Category = "AOE Charge Projectile Ability", meta = (Bitmask, BitmaskEnum = "ECollisionChannel"))
		int EffectCollisionChannel;

	// TODO: Maybe do this in projectile instead
	// Apply damage to actors in a radius
	void ApplyDamageInRadius(const FVector Origin) const;
	
protected:
	virtual void OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile) override;
};
