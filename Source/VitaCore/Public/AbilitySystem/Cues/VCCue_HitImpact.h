// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayCueNotifyObject.h"
#include "VCCue_HitImpact.generated.h"

class UCameraShake;
class UAkAudioEvent;

/**
 * 
 */
UCLASS()
class VITACORE_API UVCCue_HitImpact : public UVCGameplayCueNotifyObject
{
	GENERATED_BODY()

public:
	UVCCue_HitImpact();

protected:
	// This particle effect will be spawned at target
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		UParticleSystem* HitParticle;

	// Transform of the particle system that will be spawned
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		FTransform HitParticleTransform;

	// This is the camera shake that will be applied to target
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		TSubclassOf<UCameraShake> TargetCameraShake;

	// This is the camera shake that will be applied to source
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		TSubclassOf<UCameraShake> SourceCameraShake;

	// Sound that will be played on hit
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		UAkAudioEvent* HitAkAudioEvent;
	
protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
