// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCOverlapComponent.h"
#include "VCEntityInterface.h"

UVCOverlapComponent::UVCOverlapComponent()
	:bShouldOverlapIfAuthority(true),
	bShouldRemoveActorOnOverlapEnd(false),
	bShouldOverlapWithOwner(false),
	OverlapCollisionProfile("OverlapOnlyPawn")
{
}

void UVCOverlapComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UVCOverlapComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UVCOverlapComponent::OnOverlapEnd);
	}
	
	IVCEntityInterface* OwnerEntityInterface = Cast<IVCEntityInterface>(GetOwner());
	if (OwnerEntityInterface != nullptr)
	{
		OwnerEntity.SetInterface(OwnerEntityInterface);
		OwnerEntity.SetObject(GetOwner());
	}
}

void UVCOverlapComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OwnerEntity.GetInterface() == nullptr || OtherActor == nullptr || OtherComp == nullptr)
	{
		return;
	}

	// No not overlap with owner
	if (OtherActor == GetOwner() && bShouldOverlapWithOwner == false)
	{
		return;
	}

	const EAffiliationType OtherAffiliation = OwnerEntity->GetAffiliation(OtherActor);
	if(OtherAffiliation & OverlappingAffiliationMask)
	{
		if(OverlappedActors.Find(OtherActor) == INDEX_NONE)
		{
			OverlappedActors.Add(OtherActor);
			const FHitInfo HitInfo(GetOwner(), OtherActor, OverlappedComp, OtherComp);
			HandleOverlapActorAdded(OtherActor, OtherComp, OtherAffiliation, HitInfo);
			OnOverlapActorAdded.Broadcast(OtherActor, OtherComp, OtherAffiliation, HitInfo);
		}
	}

}

void UVCOverlapComponent::HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	const EAffiliationType OtherAffiliation, const FHitInfo HitInfo)
{
}

void UVCOverlapComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OwnerEntity == nullptr || OtherActor == nullptr || OtherComp == nullptr)
	{
		return;
	}

	// No not overlap with owner
	if(OtherActor == GetOwner() && bShouldOverlapWithOwner == false)
	{
		return;
	}

	// Remove overlapped actor from the list
	const int Index = OverlappedActors.Find(OtherActor);
	if(Index != INDEX_NONE)
	{
		const EAffiliationType OtherAffiliation = OwnerEntity->GetAffiliation(OtherActor);
		if (bShouldRemoveActorOnOverlapEnd)
		{
			OverlappedActors.RemoveAt(Index);
			HandleOverlapActorRemoved(OtherActor, OtherComp, OtherAffiliation);
			OnOverlapActorRemoved.Broadcast(OtherActor, OtherComp, OtherAffiliation);
		}
	}
}

void UVCOverlapComponent::OnClearOverlappedActors()
{
}

void UVCOverlapComponent::HandleOverlapActorRemoved(AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                         const EAffiliationType OtherAffiliation)
{
}

void UVCOverlapComponent::SetOverlapActive(bool IsActive)
{
	SetGenerateOverlapEvents(IsActive);
	IsActive ? SetCollisionProfileName(OverlapCollisionProfile) : SetCollisionProfileName("NoCollision");
}

void UVCOverlapComponent::SetOverlapRadius(float OverlapRadius)
{
	CapsuleHalfHeight = OverlapRadius;
	CapsuleRadius = OverlapRadius;
}

void UVCOverlapComponent::SetOverlappingAffiliationMask(int NewMask)
{
	OverlappingAffiliationMask = NewMask;
}

void UVCOverlapComponent::ClearOverlappedActors()
{
	OverlappedActors.Empty();
}

const TArray<AActor*>& UVCOverlapComponent::GetOverlappedActors() const
{
	return OverlappedActors;
}


