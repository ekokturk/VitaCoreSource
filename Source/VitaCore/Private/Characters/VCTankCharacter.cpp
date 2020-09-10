// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCTankCharacter.h"

#include "Net/UnrealNetwork.h"
#include "GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "VCMeleeComponent.h"

AVCTankCharacter::AVCTankCharacter()
	:MeleeComponentSocket("MetalHandSocket")
{
	MeleeComponent = CreateDefaultSubobject<UVCMeleeComponent>(TEXT("MeleeComponent"));
	MeleeComponent->SetGenerateOverlapEvents(false);
	MeleeComponent->SetCollisionProfileName("NoCollision");
	MeleeComponent->SetupAttachment(RootComponent);

	MeleeParticle = CreateDefaultSubobject<UParticleSystemComponent>("MeleeParticle");
	MeleeParticle->SetupAttachment(RootComponent);
	
}

void AVCTankCharacter::BeginPlay()
{
	Super::BeginPlay();
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	if(MeleeComponent)
	{
		MeleeComponent->AttachToComponent(GetMesh(), AttachmentRules, MeleeComponentSocket);
	}
	if(MeleeParticle)
	{
		MeleeParticle->AttachToComponent(GetMesh(), AttachmentRules, MeleeComponentSocket);
	}
}


