// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayCueNotifyActor.h"
#include "VCCue_AttachedParticle.generated.h"

/**
 * This gameplay cue creates a managing actor that spawns and handles a particle systems
 */

class AVCCharacter;
class UAkAudioEvent;

// This struct defines properties required by spawned particles for Attached Particle Cue
USTRUCT()
struct FVCCue_AttachParticleProps
{
	GENERATED_BODY()

	FVCCue_AttachParticleProps();

	// Particle to be spawned on execution
	UPROPERTY(EditAnywhere)
		UParticleSystem* Particle;

	// If this is true, particle will be attached to the scene component
	UPROPERTY(EditAnywhere)
		bool bShouldAttachParticle;

	// Attachment type of particle system
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EAttachLocation::Type> ParticleAttachmentType;
	
	// Transform of the particle system that will be spawned
	UPROPERTY(EditAnywhere)
		FTransform ParticleRelativeTransform;

	// If this is larger than 0, it will offset the spawn location of the particle by actor bounding box height times this value
	UPROPERTY(EditAnywhere)
		float BoundingBoxHeightOffsetMagnitude;

	// Sound that will be played
	UPROPERTY(EditAnywhere)
		UAkAudioEvent* AkEvent;

	// Color parameter for changing team
	UPROPERTY(EditAnywhere)
		FName ColorParameterName;
	

	bool IsValid() const { return Particle != nullptr; }
	void PlaySound(AActor* Context) const;
	UParticleSystemComponent* Spawn(USceneComponent* SpawnComponent, float BoundingBoxHeight = 0.f,
		FName AttachmentSocket = NAME_None, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None) const;
};

UCLASS()
class VITACORE_API AVCCue_AttachedParticle : public AVCGameplayCueNotifyActor
{
	GENERATED_BODY()
public:
	AVCCue_AttachedParticle();

// ================== ATTACHMENT OPTIONS ==================
protected:
	// If this is used, particle will attach to the Character Mesh that has this tag. If not found Root Component will be used.
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle|AttachmentOptions")
		bool bShouldAttachToCharacterMesh;
	
	// If this is used, particle will attach to the Scene Component to be attached will be used that has this tag. If not found Root Component will be used.
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle|AttachmentOptions", meta=(EditCondition="!bShouldAttachToCharacterMesh"))
		FName AttachedSceneComponentTag;

	// Socket that will be used on scene component for the attachment
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle|AttachmentOptions")
		FName AttachmentParticleSocketName;

	// Pooling method of the spawned particles
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle|AttachmentOptions")
		EPSCPoolMethod ParticlePoolingMethod;
	
	// If true particle color for attached particle will be set to instigator team color
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle|AttachmentOptions")
		bool bShouldParticlesUseInstigatorTeamColor;

	// Properties of the particle that will be spawned on execution
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle")
		FVCCue_AttachParticleProps ExecutionProps;

	// Properties of the particle that will be spawned on activation
	// This particle will be looping until it is removed with the tag
	// When bAllowMultipleOnActiveEvents is true, currently looping particle will reset
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle")
		FVCCue_AttachParticleProps ActivationProps;

	// Properties of the particle that will be spawned on removal
	UPROPERTY(EditAnywhere, Category = "Cue - AttachedParticle")
		FVCCue_AttachParticleProps RemovalProps;

	
protected:
	// Cached particle that will be spawned on activation
	UPROPERTY()
		UParticleSystemComponent* LoopingParticle;

	// Cached component where particles will be spawned form
	UPROPERTY()
		USceneComponent* TargetAttachmentComponent;

	// Color of the team
	UPROPERTY()
		FLinearColor TeamColor;

	// Height of the actor bounding box
	UPROPERTY()
		float BoundingBoxHeight;

	// Cache the scene component to attach the particle or spawn the particle from
	UFUNCTION()
		void SetAttachmentComponent(AActor* AttachmentTarget);
	
protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;


};
