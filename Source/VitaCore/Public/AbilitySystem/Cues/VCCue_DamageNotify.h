// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayCueNotifyObject.h"
#include "VCCue_DamageNotify.generated.h"

class UCameraShake;

/**
 * 
 */
UCLASS()
class VITACORE_API UVCCue_DamageNotify : public UVCGameplayCueNotifyObject
{
	GENERATED_BODY()
public:
	UVCCue_DamageNotify();

protected:
	// This is the camera shake that will be applied to source
	UPROPERTY(EditAnywhere, Category = "Cue - HitImpact")
		TSubclassOf<UCameraShake> SourceCameraShake;
	
protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
};
