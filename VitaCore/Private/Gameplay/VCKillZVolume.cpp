// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCKillZVolume.h"
#include "VCEntityInterface.h"
#include "Components/BoxComponent.h"

// Sets default values
AVCKillZVolume::AVCKillZVolume()
{
	KillVolumeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("KillZoneCollider"));
	KillVolumeCollider->SetBoxExtent(FVector(100,100,50));
	KillVolumeCollider->SetCollisionProfileName("OverlapAll");
	KillVolumeCollider->SetEnableGravity(false);
	KillVolumeCollider->SetGenerateOverlapEvents(true);
	RootComponent = KillVolumeCollider;
}

// Called when the game starts or when spawned
void AVCKillZVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AVCKillZVolume::OnOverlapBegin);
}

void AVCKillZVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if (OtherActor == nullptr)
		{
			return;
		}
		IVCEntityInterface * GameEntity = Cast<IVCEntityInterface>(OtherActor);
		if (GameEntity != nullptr)
		{
			GameEntity->DoDeath();
			return;
		}
		OtherActor->Destroy();
	}
}


