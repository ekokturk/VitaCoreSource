// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCPickup.h"

#include "Components/SphereComponent.h"
#include "VCCoreGameState.h"
#include "VCCharacter.h"
#include "AbilitySystemComponent.h"
#include "VCGameLibrary.h"
#include "VCSpawner.h"
#include "VCTeam.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVCPickup::AVCPickup()
	:bIsOwnTeamCollecting(false),
	bIsTeamEffect(true)
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetSphereRadius(25.0f);
	CollisionComponent->SetEnableGravity(false);
	CollisionComponent->SetCollisionProfileName("OverlapPawnIgnoreAll");
	RootComponent = CollisionComponent;

	// Static Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AVCPickup::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AVCPickup::OnOverlapBegin);
}

void AVCPickup::DoCollect(AVCCharacter* OtherCharacter)
{	
	BP_OnCollect();
	if(GetLocalRole() == ROLE_Authority)
	{
		if(Effect)
		{
			ApplyEffect(OtherCharacter);
		}
		Destroy();
	}
}

void AVCPickup::ApplyEffect(AVCCharacter* OtherCharacter) const
{
	if (Effect == nullptr || OtherCharacter == nullptr){
		return;
	}

	UAbilitySystemComponent * OtherAbilitySystemComponent = OtherCharacter->GetAbilitySystemComponent();
	if(OtherAbilitySystemComponent == nullptr){
		return;
	}
	
	const FGameplayEffectContextHandle ContextHandle = OtherAbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle Handle = OtherAbilitySystemComponent->MakeOutgoingSpec(Effect, EffectLevel, ContextHandle);
	
	if(bIsTeamEffect == true)
	{
		AVCTeam* OtherCharacterTeam = OtherCharacter->GetTeam();
		if(OtherCharacterTeam)
		{
			OtherCharacterTeam->ApplyTeamEffect(Handle);
		}
	}
	else
	{
		OtherAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
	}
}

void AVCPickup::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AVCCharacter * OtherCharacter = Cast<AVCCharacter>(OtherActor);
	if(OtherCharacter)
	{
		if(OtherCharacter->GetTeamType() == TeamType && bIsOwnTeamCollecting == false)
		{
			return;
		}
		else if(OtherCharacter->GetTeamType() != TeamType && bIsOwnTeamCollecting == true)
		{
			return;
		}
		DoCollect(OtherCharacter);
	}
	
}

