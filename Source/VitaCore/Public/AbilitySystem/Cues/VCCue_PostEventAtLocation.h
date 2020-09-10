// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayCueNotifyObject.h"
#include "VCCue_PostEventAtLocation.generated.h"

class UAkAudioEvent;

/*
 * This GameplayCue object is used to post an Ak Audio event to targeted actor
 */

UCLASS()
class VITACORE_API UVCCue_PostEventAtLocation : public UVCGameplayCueNotifyObject
{
	GENERATED_BODY()
public:
	UVCCue_PostEventAtLocation();

protected:
	// Ak audio event that will be triggered with cue
	UPROPERTY(EditAnywhere, Category = "Cue - PostEventAtLocation")
		UAkAudioEvent* AkEvent;

// ================== CUE OBJECT ============================
protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
};
