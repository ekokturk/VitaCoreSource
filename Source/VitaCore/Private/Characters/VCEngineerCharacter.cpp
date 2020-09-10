// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCEngineerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AkGameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/VCShooterComponent.h"
#include "VCEffectComponent.h"
#include "UnrealNetwork.h"


AVCEngineerCharacter::AVCEngineerCharacter():
	LaserSocket(TEXT("LaserSocket")),
	LeftJetpackThrusterSocket(TEXT("LeftJetpackThrusterSocket")),
	RightJetpackThrusterSocket(TEXT("RightJetpackThrusterSocket"))
{
	
	// Jetpack Mesh
	JetpackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("JetpackMesh"));
	JetpackMesh->SetEnableGravity(false);
	JetpackMesh->SetGenerateOverlapEvents(false);
	JetpackMesh->SetCollisionProfileName("NoCollision");
	JetpackMesh->SetupAttachment(GetMesh(), JetpackSocket);
	

	// Laser shooter component
	JetpackLaserShooterComponent = CreateDefaultSubobject<UVCShooterComponent>(TEXT("LaserShooterComponent"));
	JetpackLaserShooterComponent->SetIsReplicated(true);
	JetpackMesh ?	JetpackLaserShooterComponent->SetupAttachment(JetpackMesh, LaserSocket) :
					JetpackLaserShooterComponent->SetupAttachment(RootComponent);

	// Flamethrowe shooter component
	FlamethrowerEffectComponent = CreateDefaultSubobject<UVCEffectComponent>(TEXT("FlamethrowerEffectComponent"));
	FlamethrowerEffectComponent->SetOverlappingAffiliationMask( 0 | EAffiliationType::ENEMY);
	FlamethrowerEffectComponent->SetCapsuleHalfHeight(180.0f);
	FlamethrowerEffectComponent->SetCapsuleRadius(60.0f);
	FlamethrowerEffectComponent->SetOverlapActive(false);
	FlamethrowerEffectComponent->SetupAttachment(GetMesh(), MuzzleSocketName);
	FlamethrowerEffectComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	FlamethrowerEffectComponent->SetRelativeLocation(FVector(FlamethrowerEffectComponent->GetUnscaledCapsuleHalfHeight(), 0.0f, 0.0f));
	

	LeftJetpackThrusterParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftJetpackThrusterParticle"));
	LeftJetpackThrusterParticle->SetupAttachment(JetpackMesh, LeftJetpackThrusterSocket);

	RightJetpackThrusterParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightJetpackThrusterParticle"));
	RightJetpackThrusterParticle->SetupAttachment(JetpackMesh, RightJetpackThrusterSocket);

}

void AVCEngineerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(AkEvent_EngineerIdle)
	{
		UAkGameplayStatics::PostEvent(AkEvent_EngineerIdle, this, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>());
	}
}


void AVCEngineerCharacter::HandleDeathStateChange(bool IsDead)
{
	Super::HandleDeathStateChange(IsDead);
	
	if (IsDead)
	{
		if (LeftJetpackThrusterParticle)
		{
			LeftJetpackThrusterParticle->Deactivate();
		}
		if (RightJetpackThrusterParticle)
		{
			RightJetpackThrusterParticle->Deactivate();
		}
	}
	else
	{
		if (LeftJetpackThrusterParticle)
		{
			LeftJetpackThrusterParticle->Activate();
		}
		if (RightJetpackThrusterParticle)
		{
			RightJetpackThrusterParticle->Activate();
		}
	}
}
