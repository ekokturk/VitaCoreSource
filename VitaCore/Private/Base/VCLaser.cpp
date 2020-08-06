// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCLaser.h"

#include "UnrealNetwork.h"
#include "TimerManager.h"

#include "Components\SphereComponent.h"
#include "Components\SceneComponent.h"

#include "AbilitySystemInterface.h" // For Getting Component
#include "AbilitySystemComponent.h"
#include "AkGameplayStatics.h"

#include "VCHeroCharacter.h"
#include "VCGameLibrary.h"

// Sets default values
AVCLaser::AVCLaser()
	:bShouldDieOnStop(true),
	DeathDelay(1.0f),
	LaserDuration(3.0f),
	LaserLength(2000.0f),
	LaserLengthVectorParameter("Laser_Length")
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	// Base Collider
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	if(CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(25.0f);
		CollisionComponent->SetCollisionProfileName("OverlapOnlyPawn");
		CollisionComponent->SetupAttachment(RootComponent);
	}

	LaserBeamParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserBeamParticle"));
	if(LaserBeamParticle)
	{
		LaserBeamParticle->SetupAttachment(RootComponent);
	}
	
	LaserImpactParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserImpactParticle"));
	if(LaserImpactParticle)
	{
		LaserImpactParticle->SetupAttachment(RootComponent);
	}

	// ------------- Initialization
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bIsDestroyedOnDeath = true;
	EntityLifetime = 0.0f;
}

void AVCLaser::SetLaserDuration(float NewDuration)
{
	LaserDuration = NewDuration;
}

void AVCLaser::SetLaserLength(float NewLength)
{
	LaserLength = NewLength;
}

void AVCLaser::PostActorCreated()
{
	Super::PostActorCreated();
	if (GetLocalRole() == ROLE_Authority)
	{
		// Initialize Team
		IVCEntityInterface* MyInstigatorEntity = Cast<IVCEntityInterface>(GetInstigator());
		if (MyInstigatorEntity != nullptr)
		{
			TeamType = MyInstigatorEntity->GetTeamType();
		}
	}
}


void AVCLaser::BeginPlay()
{
	Super::BeginPlay();
	
	// Only collide on the server
	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AVCLaser::OnOverlapBegin);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AVCLaser::OnOverlapEnd);
	}
	
	if(UWorld * World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle_LaserDuration,this, &AVCLaser::StopLaser, LaserDuration, false, -1);
	}

	// Play laser init sound
	UAkGameplayStatics::PostEventAttached(LaserInitAkEvent, this, NAME_None, true);
}

void AVCLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	FHitResult OutHit;
	const FCollisionQueryParams CollisionParams(FName("LaserTrace"),false, GetInstigator());
	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * LaserLength, ECC_Pawn, CollisionParams);

	const FVector ImpactLocation = IsHit ? OutHit.ImpactPoint : GetActorLocation() + GetActorForwardVector() * LaserLength;
	const float ReachDistance = IsHit ? OutHit.Distance : LaserLength;
	
	if(IsHit)
	{
		if(LaserImpactParticle != nullptr )
		{
			if(LaserImpactParticle->IsActive() == false)
			{
				LaserImpactParticle->Activate();
			}
			LaserImpactParticle->SetWorldLocation(ImpactLocation);
		}
	}
	else
	{
		if(LaserImpactParticle != nullptr)
		{
			LaserImpactParticle->Deactivate();
		}

	}
	
	CollisionComponent->SetWorldLocation(ImpactLocation);
	if(LaserBeamParticle != nullptr)
	{
		LaserBeamParticle->SetVectorParameter(LaserLengthVectorParameter, FVector(ReachDistance, ReachDistance, ReachDistance));
	}
}

void AVCLaser::StopLaser()
{
	SetActorTickEnabled(false);
	// Deactivate laser
	OnLaserStopped.Broadcast();
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->SetCollisionProfileName("NoCollision");
	if (LaserImpactParticle != nullptr)
	{
		LaserImpactParticle->Deactivate();
	}
	if (LaserBeamParticle != nullptr)
	{
		LaserImpactParticle->Deactivate();
	}
	if(bShouldDieOnStop)
	{
		if(DeathDelay > 0.0f)
		{
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([=]() { if (this) DoDeath(); });
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_DeathDelay, TimerDelegate, DeathDelay, false);
		}
		else
		{
			DoDeath();
		}
	}
}

void AVCLaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Do not collide with instigator
	if (GetInstigator() == OtherActor && this == OtherActor)
	{
		return;
	}
	IAbilitySystemInterface * OtherAbilityActor = Cast<IAbilitySystemInterface>(OtherActor);
	
	if (OtherAbilityActor != nullptr && OtherAbilityActor->GetAbilitySystemComponent() != nullptr)
	{
		switch (GetAffiliation(OtherActor))
		{
			// Effect to be applied to allies
			case EAffiliationType::ALLY:
			{
				if(AllyEffectSpecHandle.IsValid())
				{
					OtherAbilityActor->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*AllyEffectSpecHandle.Data.Get());
				}
				break;
			}
			// Effect to be applied to enemies
			case EAffiliationType::ENEMY:
			{
				if(EnemyEffectSpecHandle.IsValid())
				{
					OtherAbilityActor->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EnemyEffectSpecHandle.Data.Get());
				}
				break;
			}
			default:break;
		}
	}
}


void AVCLaser::OnOverlapEnd(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Remove Effect from enemy
	if (GetInstigator() == OtherActor && this == OtherActor)
	{
		return;
	}
	IAbilitySystemInterface* OtherAbilityActor = Cast<IAbilitySystemInterface>(OtherActor);
	IAbilitySystemInterface* InstigatorAbilityActor = Cast<IAbilitySystemInterface>(GetInstigator());

	if (OtherAbilityActor != nullptr && OtherAbilityActor->GetAbilitySystemComponent() != nullptr && InstigatorAbilityActor != nullptr)
	{
		switch (GetAffiliation(OtherActor))
		{
			// Effect to be removed from allies
			case EAffiliationType::ALLY:
			{
				if(AllyEffectSpecHandle.IsValid())
				{
					const UGameplayEffect* AlliedEffect = AllyEffectSpecHandle.Data->Def;
					OtherAbilityActor->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(AlliedEffect->GetClass(),
						InstigatorAbilityActor->GetAbilitySystemComponent());
				}
				break;
			}
			// Effect to be removed from enemies
			case EAffiliationType::ENEMY:
			{
				if(EnemyEffectSpecHandle.IsValid())
				{
					const UGameplayEffect* EnemyEffect = EnemyEffectSpecHandle.Data->Def;
					OtherAbilityActor->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(EnemyEffect->GetClass(), 
						InstigatorAbilityActor->GetAbilitySystemComponent());
				}
				break;
			}
			default:break;
		}
	}
}
