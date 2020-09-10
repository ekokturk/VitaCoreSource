// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_ChargeProjectileAOE.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "VCProjectile.h"

UVCAbility_ChargeProjectileAOE::UVCAbility_ChargeProjectileAOE()
	:EffectRadius(300.0f),
	EffectMagnitudeAtSplashBounds(1.0f),
	EnergySplashDamageBoostMagnitude(1.0f)
{

}

void UVCAbility_ChargeProjectileAOE::OnShooterProjectileSpawned(AVCProjectile* SpawnedProjectile)
{
	Super::OnShooterProjectileSpawned(SpawnedProjectile);
	if(SpawnedProjectile)
	{
		SpawnedProjectile->GetOnEntityDeathDelegate().AddWeakLambda(this,[=](const FDeathInfo DeathInfo){
			if(SpawnedProjectile)
			{
				ApplyDamageInRadius(SpawnedProjectile->GetActorLocation());
			}
		});
	}
}

void UVCAbility_ChargeProjectileAOE::ApplyDamageInRadius(const FVector Origin) const
{
	if(UWorld * World = GetWorld())
	{
		// Clamp effect decay
		const float ClampedEffectMagnitudeAtSplashBounds = FMath::Clamp(EffectMagnitudeAtSplashBounds, 0.0f, 1.0f);

		// Check actors in radius for the splash damage
		TArray<FOverlapResult> OverlapResults;
		const FCollisionObjectQueryParams CollisionQueryParams = FCollisionObjectQueryParams(EffectCollisionChannel);
		
		if (World->OverlapMultiByObjectType(OverlapResults, Origin,FQuat::Identity, CollisionQueryParams, 
			FCollisionShape::MakeSphere(EffectRadius)))
		{
			for (FOverlapResult& OverlapResult : OverlapResults)
			{
				// Exclude owner actor
				if(OverlapResult.GetActor() == GetCurrentActorInfo()->OwnerActor || GetAffiliation(OverlapResult.GetActor()) != EAffiliationType::ENEMY)
				{
					continue;
				}
				
				// Get the distance of the actor primitive collision point from effect origin
				FVector ClosestPoint;
				const float Distance = OverlapResult.GetComponent()->GetDistanceToCollision(Origin, ClosestPoint);
				// Get distance ratio of damage origin
				const float DistanceRatio = FMath::Clamp((EffectRadius - Distance) / EffectRadius, 0.0f, 1.0f);

				// Get the decayed damage based on where the actor is from the effect origin and decay ratio
				const float DecayedDamageMagnitude = FMath::Clamp(DistanceRatio, ClampedEffectMagnitudeAtSplashBounds, 1.0f);

				IAbilitySystemInterface * OverlapActorAbilitySystem = Cast<IAbilitySystemInterface>(OverlapResult.GetActor());
				if (OverlapActorAbilitySystem && OverlapActorAbilitySystem->GetAbilitySystemComponent())
				{
					FGameplayEffectSpecHandle SplashEffectSpecHandle = MakeOutgoingGameplayEffectSpec(SplashEffect, GetOwnerAbilityLevel());

					// Apply splash damage with the radius decay
					UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SplashEffectSpecHandle, EnergyDamageBoostDataTag, 
						GetUsedEnergy() * EnergySplashDamageBoostMagnitude * DecayedDamageMagnitude);

					// Apply effect to target
					GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SplashEffectSpecHandle.Data.Get(),
						OverlapActorAbilitySystem->GetAbilitySystemComponent());
						
				}
			}
		}
	}
}
