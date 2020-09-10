// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "AnimNotify_VCSetFullBody.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "VCAnimInstance.h"

void UAnimNotify_VCSetFullBody::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp == nullptr)
	{
		return;
	}

	UVCAnimInstance * AnimInstance = Cast<UVCAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance != nullptr)
	{
		AnimInstance->ShouldDoFullBody = ShouldDoFullBody;
	}

}