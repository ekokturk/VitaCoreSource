// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_ProjectileAreaEffect.h"
#include "VCProjectile.h"
#include "VCAreaVolume.h"

UVCAbility_ProjectileAreaEffect::UVCAbility_ProjectileAreaEffect()
{
}

void UVCAbility_ProjectileAreaEffect::OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile)
{
	UVCAbility_Projectile::OnShooterProjectileSpawned(SpawnedProjectile);
	if(SpawnedProjectile)
	{
		SpawnedProjectile->GetOnEntityDeathDelegate().AddWeakLambda(this, [=](const FDeathInfo DeathInfo)
		{
			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Instigator = SpawnedProjectile->GetInstigator();
				SpawnParameters.Owner = SpawnedProjectile->GetInstigator();
				World->SpawnActor<AVCAreaVolume>(EffectArea, SpawnedProjectile->GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
			}
		});
	}
}
