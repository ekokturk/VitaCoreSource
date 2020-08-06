// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "Cues/VCCue_HitImpact.h"

#include "AkGameplayStatics.h"
#include "Camera/CameraShake.h"
#include "Kismet/GameplayStatics.h"
#include "VCCharacter.h"
#include "VCPlayerController.h"

UVCCue_HitImpact::UVCCue_HitImpact()
{
	
}

bool UVCCue_HitImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if(Parameters.EffectContext.IsValid())
	{
		
		if(Parameters.EffectContext.GetHitResult())
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle,
				Parameters.EffectContext.GetHitResult()->ImpactPoint + HitParticleTransform.GetLocation(),
				HitParticleTransform.Rotator(),
				HitParticleTransform.GetScale3D());

			// Apply camera shake to target
			if(TargetCameraShake)
			{
				AVCCharacter* MyTargetCharacter = Cast<AVCCharacter>(MyTarget);
				if(MyTargetCharacter && MyTargetCharacter->GetPlayerController())
				{
					MyTargetCharacter->GetPlayerController()->ClientPlayCameraShake(TargetCameraShake);
				}
			}

			// Apply camera shake to source
			if(SourceCameraShake)
			{
				AVCCharacter* InstigatorCharacter = Cast<AVCCharacter>(Parameters.EffectContext.GetInstigator());
				if (InstigatorCharacter && InstigatorCharacter->GetPlayerController())
				{
					InstigatorCharacter->GetPlayerController()->ClientPlayCameraShake(TargetCameraShake);
				}
			}
			
			UAkGameplayStatics::PostEventAtLocation(HitAkAudioEvent, Parameters.EffectContext.GetHitResult()->ImpactPoint, FRotator::ZeroRotator, FString(TEXT("")), MyTarget);

		}
	}
	
	return true;
}
