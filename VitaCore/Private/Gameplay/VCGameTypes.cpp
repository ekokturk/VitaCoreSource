// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCGameTypes.h"


#include "VCEntityInterface.h"
#include "Components/PrimitiveComponent.h"

FHitInfo::FHitInfo()
{
}

FHitInfo::FHitInfo(AActor* Source, AActor* Target, UPrimitiveComponent* SourceComp, UPrimitiveComponent* TargetComp)
	:SourceActor(Source),
	TargetActor(Target),
	SourceComponent(SourceComp),
	TargetComponent(TargetComp)
{
	if(TargetComponent)
	{
		if(SourceComp)
		{
			TargetComponent->GetClosestPointOnCollision(SourceComp->GetComponentLocation(), ImpactLocation);
		}
		else if(Source)
		{
			TargetComponent->GetClosestPointOnCollision(Source->GetActorLocation(), ImpactLocation);
		}
	}
}

const FHitResult FHitInfo::ConvertToHitResult() const
{
	FHitResult HitResult;
	HitResult.Actor = TargetActor;
	HitResult.Component = TargetComponent;
	HitResult.ImpactPoint = ImpactLocation;
	HitResult.Location = SourceComponent ? SourceComponent->GetComponentLocation() : FVector::ZeroVector;
	HitResult.Distance = SourceComponent ? FVector::Distance(SourceComponent->GetComponentLocation(), ImpactLocation) : 0.f;

	return HitResult;
}

//void FHitInfo::AddHitResult(const FHitResult& InHitResult)
//{
//	if(HitResult.IsValid())
//	{
//		HitResult.Reset();
//	}
//	HitResult = MakeShared<FHitResult>(FHitResult(InHitResult));
//}

bool FDeathInfo::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint8 bIsDead = IsDead;
	Ar.SerializeBits(&bIsDead, 1);
	IsDead = bIsDead & 1;
	
	Ar << KillerActor;
	
	if(Ar.IsLoading())
	{
		// Initialize killer team
		SetKillerTeamType();
	}

	bOutSuccess = true;
	return true;
}

void FDeathInfo::SetKillerTeamType()
{
	IVCEntityInterface* KillerEntity = Cast<IVCEntityInterface>(KillerActor);
	if(KillerActor)
	{
		KillerTeamType = KillerEntity->GetTeamType();
	}
}

FSpawnAroundParams::FSpawnAroundParams():
	Distance(200.0f),
	AngleBetween(0.0f),
	AngleOffset(0.0f),
	bShouldClampMaxAngle(false),
	MaxAngle(0.0f)
{
}
