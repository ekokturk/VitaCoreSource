// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_Projectile.h"
#include "Animation/AnimInstance.h"
#include "Components/VCShooterComponent.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "VCProjectile.h"


UVCAbility_Projectile::UVCAbility_Projectile()
	:ShooterComponentTag(NAME_None)
{
	bShouldAnimationStopWhenAbilityEnds = false;
	bShouldEndAbilityWithAnimation = false;
}


void UVCAbility_Projectile::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ActorShooterComponents = GetAbilityActorComponents<UVCShooterComponent>(ActorInfo, ShooterComponentTag);
}


void UVCAbility_Projectile::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	if (HasAuthority(&ActivationInfo) == true && Projectile != nullptr)
	{
		// Create Effect handle to pass to projectile
		if (ProjectileEffect != nullptr)
		{
			ProjectileParams.GameplayEffectHandle = MakeOutgoingGameplayEffectSpec(ProjectileEffect, GetOwnerAbilityLevel());
		}

		// Shoot projectiles
		for (auto& ShooterComponent : ActorShooterComponents)
		{
			// End ability when shooting ends
			ShooterComponent->OnShooterProjectileSpawned.AddUObject(this, &UVCAbility_Projectile::OnShooterProjectileSpawned);

			ShooterComponent->OnShootingStopped.AddWeakLambda(this,[=]() {
				OnShooterStoppedShooting(ShooterComponent);
			});
			
			ShooterComponent->Shoot(Projectile, ProjectileParams);

			// Only shoot from single component
			if (ShouldUseMultipleShooterComponent == false)
			{
				break;
			}
		}

	}
}
void UVCAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle,
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


void UVCAbility_Projectile::OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile)
{
	
}

void UVCAbility_Projectile::OnShooterStoppedShooting(UVCShooterComponent* ShooterComponent)
{
	EndCurrentAbility();
}
