// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCMeleeComponent.h"

#include "VCEntityInterface.h"

UVCMeleeComponent::UVCMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetOverlappingAffiliationMask(0 | EAffiliationType::ENEMY);
	SetOverlapActive(false);
}

void UVCMeleeComponent::HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation, const FHitInfo HitInfo)
{
	OnMeleeHit.Broadcast(HitInfo);

	// TODO: Maybe for cosmetics + replication?
	//OtherVCEntity->DoHit(HitData);
	
}


