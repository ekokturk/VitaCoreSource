// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_AreaEffect.h"
#include "VCAreaVolume.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitGameplayEvent.h"
#include "VCCharacter.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"

UVCAbility_AreaEffect::UVCAbility_AreaEffect():
	TargetType(EVCAbility_AreaEffect_TargetTypes::Self),
	bShouldSpawnAround(false),
	Count(1)
{
	bShouldEndAbilityWithAnimation = true;
	bShouldAnimationStopWhenAbilityEnds = true;
}

void UVCAbility_AreaEffect::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);
	
	// Gameplay task to spawn effect area
	if (bShouldSpawnWithTag == true)
	{
		if (HasAuthority(&ActivationInfo) && EffectArea != nullptr)
		{
			Task_WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTagToSpawn);
			Task_WaitGameplayEvent->EventReceived.AddDynamic(this, &UVCAbility_AreaEffect::SpawnEffectArea);
			Task_WaitGameplayEvent->ReadyForActivation();
		}
	}
}

void UVCAbility_AreaEffect::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (bShouldSpawnWithTag == false)
	{
		if (HasAuthority(&ActivationInfo) == true)
		{
			SpawnEffectArea();
		}
	}
}


void UVCAbility_AreaEffect::SpawnEffectArea(FGameplayEventData Payload)
{
	AActor * OwningActor = GetActorInfo().OwnerActor.Get();
	if (OwningActor != nullptr)
	{
		UWorld * World = GetWorld();
		if(World == nullptr)
		{
			return;
		}
		
		FActorSpawnParameters SpawnParameters;
		// TODO : Check actor has instigator and get it's pawn
		SpawnParameters.Instigator =  Cast<APawn>(OwningActor);

		for(auto TargetActor : GetTargetActors())
		{
			const FVector ActorLocation = TargetActor->GetActorLocation();
			if(bShouldSpawnAround == true)
			{
				const FVector DistanceVector = TargetActor->GetActorForwardVector() * SpawnAroundParams.Distance;
				for(uint32 i = 0; i < Count; i++)
				{
					const FVector SpawnLocation = DistanceVector.RotateAngleAxis(SpawnAroundParams.AngleOffset + i * SpawnAroundParams.AngleBetween, FVector::UpVector);
					World->SpawnActor<AVCAreaVolume>(EffectArea, SpawnLocation + ActorLocation, TargetActor->GetActorRotation(), SpawnParameters);
				}
			}
			else
			{
				World->SpawnActor<AVCAreaVolume>(EffectArea, ActorLocation, TargetActor->GetActorRotation(), SpawnParameters);
			}
		}
	}
}

TArray<AActor*> UVCAbility_AreaEffect::GetTargetActors() const
{
	TArray<AActor*> TargetActors;

	// TODO IMPORTANT - Instead of actor iterator characters can be cached to somewhere
	switch (TargetType)
	{
		case EVCAbility_AreaEffect_TargetTypes::Self:
		{
			TargetActors.Add(GetActorInfo().OwnerActor.Get());
			break;
		}
		case EVCAbility_AreaEffect_TargetTypes::AllEnemyCharacters:
		{
			for (TActorIterator<AVCCharacter> Character(GetWorld()); Character; ++Character)
			{
				if(Character->GetAffiliation(GetActorInfo().OwnerActor.Get()) == EAffiliationType::ENEMY)
				{
					TargetActors.Emplace(*Character);
				}
			}
			break;
		}
		case EVCAbility_AreaEffect_TargetTypes::AllAllyCharacters:
		{
			for (TActorIterator<AVCCharacter> Character(GetWorld()); Character; ++Character)
			{
				if (Character->GetAffiliation(GetActorInfo().OwnerActor.Get()) == EAffiliationType::ALLY)
				{
					TargetActors.Emplace(*Character);
				}
			}
			break;
		}
		default: ;
	}


	return TargetActors;
}

