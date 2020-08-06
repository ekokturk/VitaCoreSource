// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCSpawner.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/CollisionProfile.h"
#include "TimerManager.h"
#include "VCEntityInterface.h"

AVCSpawner::AVCSpawner()
	:bShouldSpawnAtBeginPlay(true),
	bIsActive(true)
{
	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SpawnCapsule"));
	SpawnCapsule->SetCapsuleHalfHeight(100.0f);
	SpawnCapsule->SetCapsuleRadius(25.0f);
	SpawnCapsule->SetCollisionProfileName("NoCollision");
	SpawnCapsule->SetupAttachment(RootComponent);
	SpawnCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, SpawnCapsule->GetScaledCapsuleHalfHeight()));

	SpawnSprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	#if WITH_EDITORONLY_DATA
	if (SpawnSprite)
	{
			SpawnSprite->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
			SpawnSprite->bHiddenInGame = true;
			SpawnSprite->SetupAttachment(SpawnCapsule);
			SpawnSprite->SetUsingAbsoluteScale(true);
			SpawnSprite->bIsScreenSizeScaled = true;
	}
	#endif 
}

bool AVCSpawner::GetTeamType(ETeamType& OutTeamType) const
{
	if(bShouldUseTeamType)
	{
		OutTeamType = TeamType;
		return true;
	}
	return false;
}

void AVCSpawner::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		if (bShouldSpawnAtBeginPlay && bIsActive)
		{
			SpawnActorClass();
		}
		else
		{
			DoSpawn();
		}
	}
}

void AVCSpawner::DoSpawn()
{
	check(HasAuthority())
	
	if (bIsActive)
	{
		UWorld * World = GetWorld();
		if (World != nullptr)
		{
			if (SpawnDelay <= 0)
			{
				SpawnActorClass();
			}
			else
			{
				World->GetTimerManager().ClearTimer(TimerHandle_SpawnTimer);
				World->GetTimerManager().SetTimer(TimerHandle_SpawnTimer, this, &AVCSpawner::SpawnActorClass, SpawnDelay);
			}
		}
	}
}

void AVCSpawner::DoTeleport(AActor* SpawningActor) const
{
	if(GetLocalRole() == ROLE_Authority && SpawningActor && bIsActive)
	{
		// TODO Check spawn sweep and experiment with teleport type
		SpawningActor->SetActorTransform(FTransform(GetActorRotation(), GetActorLocation(), FVector(1.f,1.f,1.f)),
			false, nullptr, ETeleportType::TeleportPhysics);
	}
}

FTransform AVCSpawner::GetSpawnTransform() const
{
	return FTransform(GetActorRotation(), GetActorLocation(), FVector(1.f, 1.f, 1.f));
}

void AVCSpawner::SpawnActorClass()
{
	if (ActorClassToSpawn != nullptr)
	{
		UWorld * World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnerRotation = GetActorRotation();
			const FVector SpawnerLocation = GetActorLocation();
			FActorSpawnParameters SpawnParam;
			AActor * SpawnedActor = World->SpawnActor<AActor>(ActorClassToSpawn, SpawnerLocation, SpawnerRotation, SpawnParam);

			HandleAfterSpawnAction(SpawnedActor);
		}

	}
}

void AVCSpawner::HandleAfterSpawnAction(AActor* SpawnedActor)
{
	switch (AfterSpawnActionType)
	{
	case EAfterSpawnActionTypes::NoAction:
	{
		break;
	}
	case EAfterSpawnActionTypes::Destroy:
	{
		Destroy();
		break;
	}
	case EAfterSpawnActionTypes::Deactivate:
	{
		bIsActive = true;
		break;
	}
	case EAfterSpawnActionTypes::Respawn_OnDestroy:
	{
		if(SpawnedActor)
		{
			SpawnedActor->OnDestroyed.AddDynamic(this, &AVCSpawner::SpawnOnSpawnedActorDestroyed);
		}
		break;
	}
	case EAfterSpawnActionTypes::Respawn_OnDeath:
	{
		if(IVCEntityInterface * SpawnedEntity = Cast<IVCEntityInterface>(SpawnedActor))
		{
			SpawnedEntity->GetOnEntityDeathDelegate().AddWeakLambda(this, [=](FDeathInfo DeathInfo)
			{
				DoSpawn();
			});
		}
		break;
	}
	default:;
	}
}

void AVCSpawner::SpawnOnSpawnedActorDestroyed(AActor* DestroyedActor)
{
	DoSpawn();
}
