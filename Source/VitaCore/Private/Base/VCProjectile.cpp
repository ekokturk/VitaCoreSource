// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCProjectile.h"

#include "Components\SphereComponent.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"

#include "AbilitySystemInterface.h" // For Getting Component
#include "AbilitySystemComponent.h"
#include "VCGameLibrary.h"

#include "VCHeroCharacter.h"
#include "UnrealNetwork.h"

// Sets default values
AVCProjectile::AVCProjectile()
	:InitialSpeed(800.0f),
	ShouldDieOnOverlap(true),
	bShouldOverlapWithAllies(true),
	EffectAffiliationMask(0 | EAffiliationType::ENEMY)
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetSphereRadius(25.0f);
	CollisionComponent->SetCollisionProfileName("Projectile");
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetupAttachment(RootComponent);

	// Projectile movement 
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	// ------------- Initialization -------------
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bIsDestroyedOnDeath = true;
	EntityLifetime = 3.0f;
}

void AVCProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Initialize Collision
	OnActorBeginOverlap.AddDynamic(this, &AVCProjectile::OnOverlapBegin);
	
	if (GetLocalRole() == ROLE_Authority)
	{
		// Initialize Team
		IVCEntityInterface * MyInstigatorEntity = Cast<IVCEntityInterface>(GetInstigator());
		if (MyInstigatorEntity != nullptr)
		{
			TeamType = MyInstigatorEntity->GetTeamType();
		}
	}
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * InitialSpeed;
}

void AVCProjectile::OnRep_Instigator()
{
	InstigatorCharacter = Cast<AVCCharacter>(GetInstigator());
}


void AVCProjectile::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	// Do not collide with instigator
	if (OtherActor == nullptr || GetInstigator() == OtherActor){
		return;
	}

	if (GetAffiliation(OtherActor) == EAffiliationType::ALLY && bShouldOverlapWithAllies == false)
	{
		return;
	}

	IVCEntityInterface* OtherActorEntity = Cast<IVCEntityInterface>(OtherActor);
	if(OtherActorEntity)
	{
		FHitInfo HitInfo;
		OtherActorEntity->DoHit(HitInfo);
	}
	
	if (HasAuthority())
	{
		if (GetAffiliation(OtherActor) & EffectAffiliationMask)
		{
			IVCAbilitySystemInterface* OtherAbilitySystem = Cast<IVCAbilitySystemInterface>(OtherActor);
			if(OtherAbilitySystem && OtherAbilitySystem->GetAbilitySystemComponent())
			{
				OtherAbilitySystem->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*AbilityGameplayEffectSpecHandle.Data.Get());
			}
		}
	}
	if(ShouldDieOnOverlap == true)
	{
		DoDeath();
	}
}


// Replicate properties
void AVCProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AVCProjectile, InitialSpeed, COND_InitialOnly);
}

