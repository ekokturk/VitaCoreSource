// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_Laser.h"


#include "VCLaser.h"
#include "Kismet/GameplayStatics.h"
#include "VCShooterComponent.h"

UVCAbility_Laser::UVCAbility_Laser()
	:bShouldOverrideShooterComponentUpdateCondition(true)
{
	bShouldEndAbilityWithAnimation = false;
	bShouldAnimationStopWhenAbilityEnds = true;
}

void UVCAbility_Laser::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ActorShooterComponents = GetAbilityActorComponents<UVCShooterComponent>(ActorInfo, ShooterComponentTag);
}

void UVCAbility_Laser::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	if (HasAuthority(&ActivationInfo) == true && LaserClass != nullptr)
	{
		//Shoot projectiles
		for (auto ShooterComponent : ActorShooterComponents)
		{
			// Process laser actor once it is spawned in world
			ShooterComponent->OnShooterLaserSpawned.AddUObject(this, &UVCAbility_Laser::OnShooterLaserSpawned);

			if (bShouldOverrideShooterComponentUpdateCondition)
			{
				ShooterComponent->SetDirectionUpdate(EShooterComponentDirectionUpdateType::UPDATE);
			}
			// Create Effect handle to pass to projectile
			if (LaserAllyEffect != nullptr)
			{
				ShootParams.AllyEffectHandle = MakeOutgoingGameplayEffectSpec(LaserAllyEffect, GetOwnerAbilityLevel());
			}
			if (LaserEnemyEffect != nullptr)
			{
				ShootParams.EnemyEffectHandle = MakeOutgoingGameplayEffectSpec(LaserEnemyEffect, GetOwnerAbilityLevel());
			}
			ShooterComponent->Shoot(LaserClass, ShootParams);

			// Only shoot from single component
			if (ShouldUseMultipleShooterComponent == false)
			{
				break;
			}
		}

	}
}

void UVCAbility_Laser::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// Remove bound delegates and stop updating shooter component direction
	for (auto ShooterComponent : ActorShooterComponents)
	{
		ShooterComponent->OnShooterLaserSpawned.RemoveAll(this);
		if (bShouldOverrideShooterComponentUpdateCondition)
		{
			ShooterComponent->SetDirectionUpdate(EShooterComponentDirectionUpdateType::NO_UPDATE);
		}
	}
}


void UVCAbility_Laser::OnShooterLaserSpawned(AVCLaser* Laser)
{
	if (Laser != nullptr)
	{
		// End ability when laser is stopped
		Laser->OnLaserStopped.AddWeakLambda(this,[=]() {
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		});

	}
}
