// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCue_AttachedParticle.h"


#include "AkGameplayStatics.h"
#include "AkGameplayTypes.h"
#include "Components/SkeletalMeshComponent.h"


#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "VCCharacter.h"
#include "VCGameState.h"
#include "VCTeam.h"

// ======================= PROPERTIES ================================
FVCCue_AttachParticleProps::FVCCue_AttachParticleProps()
	: Particle(nullptr),
	bShouldAttachParticle(true),
	ParticleAttachmentType(EAttachLocation::KeepRelativeOffset),
	ParticleRelativeTransform(FTransform()),
	BoundingBoxHeightOffsetMagnitude(0.f),
	AkEvent(nullptr)
{
}

void FVCCue_AttachParticleProps::PlaySound(AActor * Context) const
{
	if(AkEvent)
	{
		UAkGameplayStatics::PostEvent(AkEvent, Context, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>());
	}
}

UParticleSystemComponent* FVCCue_AttachParticleProps::Spawn(USceneComponent* SpawnComponent, float BoundingBoxHeight, FName AttachmentSocket, EPSCPoolMethod PoolingMethod) const
{

	UParticleSystemComponent* SpawnedParticle = nullptr;
	if (SpawnComponent && IsValid())
	{
		if (bShouldAttachParticle) // Check if execution particle will be attached
		{
			SpawnedParticle = UGameplayStatics::SpawnEmitterAttached(Particle, SpawnComponent, AttachmentSocket,
				ParticleRelativeTransform.GetLocation(), ParticleRelativeTransform.Rotator(), ParticleRelativeTransform.GetScale3D(),
				ParticleAttachmentType, true, PoolingMethod, false);
			if (SpawnedParticle)
			{
				SpawnedParticle->AddWorldOffset(FVector(0.f, 0.f, BoundingBoxHeight * BoundingBoxHeightOffsetMagnitude * (1/SpawnComponent->GetComponentScale().Z)));
			}
		}
		else // Spawn particle in the world
		{
			SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(SpawnComponent->GetOwner(), Particle,
				SpawnComponent->GetComponentLocation() + ParticleRelativeTransform.GetLocation()
				+ BoundingBoxHeight * BoundingBoxHeightOffsetMagnitude,
				ParticleRelativeTransform.Rotator(), ParticleRelativeTransform.GetScale3D());
		}
	}
	return SpawnedParticle;
}


// ======================= ATTACHED PARTICLE CUE ================================


AVCCue_AttachedParticle::AVCCue_AttachedParticle()
	:bShouldAttachToCharacterMesh(true),
	AttachedSceneComponentTag(NAME_None),
	AttachmentParticleSocketName(NAME_None),
	bShouldParticlesUseInstigatorTeamColor(false)
{
	bAutoDestroyOnRemove = true;
	AutoDestroyDelay = 1.5f;
	bUniqueInstancePerInstigator = false;
	
}

void AVCCue_AttachedParticle::SetAttachmentComponent(AActor * AttachmentTarget)
{
	// Initialize Attachment point as root component
	TargetAttachmentComponent = AttachmentTarget->GetRootComponent();

	// Cache bounding box height for offset
	FVector Origin, BoundingBox;
	AttachmentTarget->GetActorBounds(true, Origin, BoundingBox);
	BoundingBoxHeight = BoundingBox.Z;
	
	// Get the scene component particle will be attached to
	if (bShouldAttachToCharacterMesh) // Get Mesh component of the character
	{
		ACharacter* TargetCharacter = Cast<ACharacter>(AttachmentTarget);
		if (TargetCharacter != nullptr)
		{
			TargetAttachmentComponent = TargetCharacter->GetMesh();
		}
	}
	else if (AttachedSceneComponentTag.IsValid()) // Search and get tagged component
	{
		const TArray<UActorComponent*> ActorComponent = AttachmentTarget->GetComponentsByTag(USceneComponent::StaticClass(), AttachedSceneComponentTag);
		if (ActorComponent.Num() > 0)
		{
			TargetAttachmentComponent = Cast<USceneComponent>(ActorComponent[1]);
		}
	}

	// Get Team color
	if(bShouldParticlesUseInstigatorTeamColor)
	{
		UWorld* World = GetWorld();
		AVCGameState* VCGameState = World ? World->GetGameState<AVCGameState>() : nullptr;
		IVCEntityInterface* EntityToAttach = Cast<IVCEntityInterface>(AttachmentTarget);
		if(EntityToAttach && VCGameState)
		{
			AVCTeam* Team = VCGameState->GetTeam(EntityToAttach->GetTeamType());
			if(Team)
			{
				TeamColor = Team->GetTeamColor();
			}
		}
	}
	
}

bool AVCCue_AttachedParticle::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// Cache particle if not cached
	if(TargetAttachmentComponent == nullptr)
	{
		SetAttachmentComponent(MyTarget);
	}

	ExecutionProps.PlaySound(this);
	UParticleSystemComponent * Particle = ExecutionProps.Spawn(TargetAttachmentComponent, BoundingBoxHeight, AttachmentParticleSocketName, ParticlePoolingMethod);
	if(Particle)
	{
		Particle->Activate();
		if (bShouldParticlesUseInstigatorTeamColor)
		{
			Particle->SetColorParameter(ExecutionProps.ColorParameterName, TeamColor);
		}
	}

	return true;
}

bool AVCCue_AttachedParticle::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// Cache attachment component
	SetAttachmentComponent(MyTarget);

	ActivationProps.PlaySound(this);
	// Create looping particle if it does not exist
	if(LoopingParticle == nullptr)
	{
		LoopingParticle = ActivationProps.Spawn(TargetAttachmentComponent, BoundingBoxHeight, AttachmentParticleSocketName, ParticlePoolingMethod);
		if(bShouldParticlesUseInstigatorTeamColor && LoopingParticle)
		{
			LoopingParticle->SetColorParameter(ActivationProps.ColorParameterName, TeamColor);
		}
	}

	// Activate particle
	if(LoopingParticle)
	{
		LoopingParticle->Activate(true);
	}

	return true;
}

bool AVCCue_AttachedParticle::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// Deactivate
	if(LoopingParticle)
	{
		LoopingParticle->Deactivate();
		LoopingParticle = nullptr;
	}

	RemovalProps.PlaySound(this);
	UParticleSystemComponent* Particle = RemovalProps.Spawn(TargetAttachmentComponent, BoundingBoxHeight, AttachmentParticleSocketName, ParticlePoolingMethod);
	if (Particle)
	{
		Particle->Activate();
		if (bShouldParticlesUseInstigatorTeamColor)
		{
			Particle->SetColorParameter(RemovalProps.ColorParameterName, TeamColor);
		}
	}
	
	return true;
}

