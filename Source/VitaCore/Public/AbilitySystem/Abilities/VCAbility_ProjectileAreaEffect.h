// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_Projectile.h"
#include "VCAbility_ProjectileAreaEffect.generated.h"

class AVCAreaVolume;

/*
 * This ability uses shooter component to spawn a projectile that creates an effect area volume on death
 */

UCLASS()
class VITACORE_API UVCAbility_ProjectileAreaEffect : public UVCAbility_Projectile
{
	GENERATED_BODY()
public:
	UVCAbility_ProjectileAreaEffect();

protected:
	// ------- EFFECT AREA -------------
	// Gameplay effect area that will be spawned when projectile is destroyed
	UPROPERTY(EditAnywhere, Category = "Projectile Area Effect Ability")
		TSubclassOf<AVCAreaVolume> EffectArea;
	
protected:
	virtual void OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile) override;
};
