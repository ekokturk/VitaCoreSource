// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_ChargeProjectile.h"
#include "VCProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VCEntityInterface.h"
#include "VCShooterComponent.h"

UVCAbility_ChargeProjectile::UVCAbility_ChargeProjectile():
	bShouldOverrideProjectileCountWithEnergy(false),
	ProjectileCountOverrideEnergyStep(0.0f),
	ProjectileCountOverrideOffset(0),
	ProjectileCountOverrideMultiplier(1.0f),
	bShouldIncreaseSpeedWithEnergy(false),
	ProjectileSpeedEnergyBoostMultiplier(1.0f),
	bShouldIncreaseDamageWithEnergy(false),
	EnergyDamageBoostMagnitude(1.0f)
{
}


void UVCAbility_ChargeProjectile::PostLoad()
{
	Super::PostLoad();
	DefaultInitialSpeed = ProjectileParams.InitialSpeed;
}

void UVCAbility_ChargeProjectile::DoOnChargeEnded(float ChargeDuration)
{

	// Shoot projectiles
	if (HasAuthority(&GetCurrentActivationInfoRef()) == true && Projectile != nullptr)
	{
		// Create Effect handle to pass to projectile
		if (ProjectileEffect != nullptr)
		{
			ProjectileParams.GameplayEffectHandle = MakeOutgoingGameplayEffectSpec(ProjectileEffect, GetOwnerAbilityLevel());
		}

		const float UsedEnergy = GetUsedEnergy();

		// Determine if the projectile count will be changed
		if(bShouldOverrideProjectileCountWithEnergy == true)
		{
			// Energy step determines how many projectiles have to be spawned by default depending on the used energy
			float EnergyStep = (CostEnergy = 0.0f ? UsedEnergy : CostEnergy);
			// Use custom energy step instead of cost
			if(ProjectileCountOverrideEnergyStep > 0)
			{
				EnergyStep = ProjectileCountOverrideEnergyStep;
			}
			// Override Projectile count
			ProjectileParams.Count = (int)(UsedEnergy / EnergyStep) * ProjectileCountOverrideMultiplier + ProjectileCountOverrideOffset;

		}

		// Increase projectile damage depending on the energy used
		if(bShouldIncreaseDamageWithEnergy == true)
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(ProjectileParams.GameplayEffectHandle, EnergyDamageBoostDataTag, UsedEnergy * EnergyDamageBoostMagnitude);
		}
		
		// Increase projectile base speed
		if(bShouldIncreaseSpeedWithEnergy == true)
		{
			ProjectileParams.InitialSpeed = DefaultInitialSpeed + UsedEnergy * ProjectileSpeedEnergyBoostMultiplier;
		}
		
		for (auto ShooterComponent : ActorShooterComponents)
		{
			ShooterComponent->OnShooterProjectileSpawned.AddUObject(this, &UVCAbility_ChargeProjectile::OnShooterProjectileSpawned);

			// End ability when shooting ends
			ShooterComponent->OnShootingStopped.AddWeakLambda(this,[=]()
			{
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			});
			ShooterComponent->Shoot(Projectile, ProjectileParams);

			// Only shoot from single component
			if(ShouldUseMultipleShooterComponent == false)
			{
				break;
			}
		}
	}
}

void UVCAbility_ChargeProjectile::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ActorShooterComponents = GetAbilityActorComponents<UVCShooterComponent>(ActorInfo, ShooterComponentTag);
}

void UVCAbility_ChargeProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	for (auto ShooterComponent : ActorShooterComponents)
	{
		ShooterComponent->OnShooterProjectileSpawned.RemoveAll(this);
		ShooterComponent->OnShootingStopped.RemoveAll(this);
	}
	ProjectileParams.TimerProps.Clear(GetWorld()->GetTimerManager());

}


void UVCAbility_ChargeProjectile::OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile)
{
}