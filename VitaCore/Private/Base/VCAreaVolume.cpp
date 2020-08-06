// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAreaVolume.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilityTargetTypes.h"
#include "VCGameLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "VCCharacter.h"
#include "Particles/ParticleSystemComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "VCHeroCharacter.h"
#include "VCTeam.h"

AVCAreaVolume::AVCAreaVolume()
{
	// Base Collider
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetSphereRadius(25.0f);
	CollisionComponent->SetCollisionProfileName("NoCollision");
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->SetEnableGravity(false);
	RootComponent = CollisionComponent;

	// Static Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetupAttachment(RootComponent);

	// Initialization
	AreaDecal = CreateDefaultSubobject<UDecalComponent>("AreaDecal");
	AreaDecal->SetupAttachment(RootComponent);
	AreaDecal->SetRelativeRotation(FRotator(90, 0, 0));
	AreaDecal->bDestroyOwnerAfterFade = false;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AVCAreaVolume::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		OnActorBeginOverlap.AddDynamic(this, &AVCAreaVolume::OnOverlapBegin);
		OnActorEndOverlap.AddDynamic(this, &AVCAreaVolume::OnOverlapEnd);
	}
	
	UWorld * World = GetWorld();
	if (World != nullptr)
	{
		if (EffectInitialDelay > 0.0f)
		{
			World->GetTimerManager().SetTimer(TimerHandle_EffectActivation, this, &AVCAreaVolume::ActivateAreaEffect, EffectInitialDelay);
		}
		else
		{
			ActivateAreaEffect();
		}
	}

	if (AreaDecal != nullptr)
	{
		if(EffectInitialDelay != 0)
		{
			AreaDecal->SetFadeIn(0, EffectInitialDelay / 2);
			AreaDecal->SetFadeOut(EffectInitialDelay, EffectInitialDelay / 2);
		}
		if (AreaDecal->GetMaterial(0) == nullptr)
		{
			AreaDecal->SetVisibility(false);
		}
	}
}

void AVCAreaVolume::PostActorCreated()
{
	Super::PostActorCreated();
	InstigatorCharacter = Cast<AVCCharacter>(GetInstigator());
	if(InstigatorCharacter && HasAuthority())
	{
		TeamType = InstigatorCharacter->GetTeamType();
	}
	SetInstigatorTeamColor();
}

void AVCAreaVolume::OnRep_Instigator()
{
	InstigatorCharacter = Cast<AVCCharacter>(GetInstigator());
	SetInstigatorTeamColor();
}

void AVCAreaVolume::ActivateAreaEffect()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->SetGenerateOverlapEvents(true);
		CollisionComponent->SetCollisionProfileName("OverlapPawnIgnoreAll");
	}
	BP_OnEffectActivation();
}

void AVCAreaVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (InstigatorCharacter == nullptr && OtherActor == this){
		return;
	}
	
	if (IAbilitySystemInterface * OtherAbilitySystem = Cast<IAbilitySystemInterface>(OtherActor))
	{
		if(OtherAbilitySystem->GetAbilitySystemComponent() == nullptr){
			return;
		}

		FGameplayEffectSpecHandle Handle;
		const FGameplayEffectContextHandle ContextHandle = InstigatorCharacter ?
			InstigatorCharacter->GetAbilitySystemComponent()->MakeEffectContext() : AbilitySystemComponent->MakeEffectContext();
		
		const float AbilityLevel = InstigatorCharacter ? InstigatorCharacter->GetCharacterLevel() : GetAbilityLevel();
		
		if (EnemyEffect && GetAffiliation(OtherActor) == EAffiliationType::ENEMY)
		{
			Handle = GetAbilitySystemComponent()->MakeOutgoingSpec(EnemyEffect, AbilityLevel, ContextHandle);

		}
		else if (AllyEffect && GetAffiliation(OtherActor) == EAffiliationType::ALLY)
		{
			Handle = GetAbilitySystemComponent()->MakeOutgoingSpec(AllyEffect, AbilityLevel, ContextHandle);
		}

		if(Handle.IsValid())
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*Handle.Data.Get(),
				OtherAbilitySystem->GetAbilitySystemComponent());
		}
	}
}

void AVCAreaVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsEffectRemovedOnExit == true)
	{
		if (InstigatorCharacter && Cast<IAbilitySystemInterface>(OtherActor) != nullptr)
		{
			UAbilitySystemComponent* InstigatorAbilitySystemComponent = InstigatorCharacter ? 
				InstigatorCharacter->GetAbilitySystemComponent() : AbilitySystemComponent;;
			if (EnemyEffect && GetAffiliation(OtherActor) == EAffiliationType::ENEMY)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(EnemyEffect, InstigatorAbilitySystemComponent);
			}
			else if (AllyEffect && GetAffiliation(OtherActor) == EAffiliationType::ALLY)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(AllyEffect, InstigatorAbilitySystemComponent);
			}
		}
	}
}

void AVCAreaVolume::SetInstigatorTeamColor() const
{
	if(ParticleSystemComponent == nullptr)
	{
		return;
	}
	
	if(InstigatorCharacter)
	{
		if(AVCTeam* Team = InstigatorCharacter->GetTeam())
		{
			ParticleSystemComponent->SetColorParameter(TeamColorParameterName, Team->GetTeamColor());
		}
	}
}


