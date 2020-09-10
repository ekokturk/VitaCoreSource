// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"

AVCActor::AVCActor():
	bIsAlive(true),
	TeamType(ETeamType::NEUTRAL),
	EntityLifetime(0),
	bIsDestroyedOnDeath(true)
{
	bReplicates = true;
}

void AVCActor::BeginPlay()
{
	Super::BeginPlay();
	if(EntityLifetime > 0)
	{
		if(UWorld * World = GetWorld())
		{
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this,[=]() { DoDeath(); });
			World->GetTimerManager().SetTimer(TimerHandle_EntityLifetime, TimerDelegate, EntityLifetime, false);
		}
	}
}


FString AVCActor::GetEntityName() const
{
	return "ActorEntity";
}

EAffiliationType AVCActor::GetAffiliation(AActor* Other) const
{
	if(IVCEntityInterface* OtherEntity = Cast<IVCEntityInterface>(Other))
	{
		// TODO Make Affiliation mask to check team types
		if (OtherEntity->GetTeamType() == ETeamType::NEUTRAL)
		{
			return EAffiliationType::NEUTRAL;
		}
		else
		{
			return OtherEntity->GetTeamType() == GetTeamType() ? EAffiliationType::ALLY : EAffiliationType::ENEMY;
		}
	}
	return EAffiliationType::NEUTRAL;

}

void AVCActor::DoDeath(FDeathInfo DeathInfo)
{
	if (bIsAlive == false) {
		return;
	}
	
	if(UWorld * World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_EntityLifetime);
	}
	OnEntityDeath.Broadcast(DeathInfo);
	BP_OnDeath();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsAlive = false;
		if (bIsDestroyedOnDeath == true)
		{
			Destroy();
		}
	}
}


void AVCActor::DoHit(const FHitInfo& HitInfo)
{

}


void AVCActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVCActor, TeamType); // TODO Can be removed condition
	DOREPLIFETIME(AVCActor, bIsAlive); // TODO Can be removed condition
}

