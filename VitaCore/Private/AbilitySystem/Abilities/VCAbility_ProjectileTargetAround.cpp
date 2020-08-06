// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_ProjectileTargetAround.h"

#include "AbilitySystemComponent.h"

UVCAbility_ProjectileTargetAround::UVCAbility_ProjectileTargetAround()
{
	
}


bool UVCAbility_ProjectileTargetAround::CommitCheck(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(Super::CommitCheck(Handle, ActorInfo, ActivationInfo) == false)
	{
		return false;
	}
	//
	TArray<FOverlapResult> Overlaps;
	const FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_Pawn);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetActorInfo().OwnerActor.Get());	// Ignore self
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetCurrentActorInfo()->OwnerActor->GetActorLocation(),
		FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TargetCheckRadius)))
	{
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (Cast<IAbilitySystemInterface>(OverlapResult.Actor.Get()) != nullptr)
			{
				if (GetAffiliation(OverlapResult.Actor.Get()) & TargetAffiliationMask)
				{
					TargetActors.Push(OverlapResult.Actor.Get());
				}
			}
		}
	}

	if (TargetActors.Num() == 0)
	{
		return false;
	}

	ProjectileParams.Target = TargetActors.Pop();

	return true;
	
}

void UVCAbility_ProjectileTargetAround::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	TargetIndex = 0;
	TargetActors.Empty();
	ProjectileParams.Target = nullptr;
}

void UVCAbility_ProjectileTargetAround::OnShooterStoppedShooting(UVCShooterComponent* ShooterComponent)
{
	if(TargetIndex < TargetActors.Num() && ShooterComponent)
	{
		AActor * TargetActor = TargetActors.Pop(false);
		ProjectileParams.Target = TargetActor;
		ProjectileParams.TimerProps.Clear(GetWorld()->GetTimerManager());
		ShooterComponent->Shoot(Projectile, ProjectileParams);
		return;
	}

	EndCurrentAbility();
}
