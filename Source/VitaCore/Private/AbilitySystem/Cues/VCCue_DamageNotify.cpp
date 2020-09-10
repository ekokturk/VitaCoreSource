// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCue_DamageNotify.h"

#include "VCCharacter.h"
#include "VCPlayerController.h"

UVCCue_DamageNotify::UVCCue_DamageNotify()
{
}

bool UVCCue_DamageNotify::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	AVCCharacter* TargetCharacter = Cast<AVCCharacter>(MyTarget);
	AVCCharacter* SourceCharacter = Cast<AVCCharacter>(Parameters.GetEffectCauser());

	// Actions for the Target of the damage
	if(TargetCharacter && TargetCharacter->GetPlayerController())
	{
		//TargetCharacter->GetPlayerController()->NotifyPlayerDamage(Parameters.RawMagnitude);
		// TODO Notify Self Damage
		TargetCharacter->GetPlayerController()->ClientPlayCameraShake(SourceCameraShake);
	}

	// Actions for the Source of the damage
	if(SourceCharacter && SourceCharacter->GetPlayerController())
	{
		// TODO Notify Other Damage

		// Only notify the damage for enemies
		if(SourceCharacter->GetAffiliation(TargetCharacter) == EAffiliationType::ENEMY)
		{
			SourceCharacter->GetPlayerController()->NotifyPlayerDamage(Parameters.RawMagnitude);
		}
	}

	return true;
}
