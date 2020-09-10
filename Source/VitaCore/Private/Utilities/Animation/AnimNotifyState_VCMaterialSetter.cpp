// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "AnimNotifyState_VCMaterialSetter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"
#include "Animation\AnimSequenceBase.h"

void UAnimNotifyState_VCMaterialSetter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(Animation != nullptr)
	{
		AnimationRate = Animation->RateScale;
	}
	MaxDuration = TotalDuration;
	CurrentDuration = 0;
}
void UAnimNotifyState_VCMaterialSetter::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CurrentDuration = CurrentDuration + FrameDeltaTime;
	const float LerpAlpha = FMath::Clamp(CurrentDuration * AnimationRate / MaxDuration, 0.0f, 1.0f);


	if (MeshComp != nullptr)
	{
		for (int i = 0; i < MeshComp->GetNumMaterials(); i++)
		{
			UMaterialInterface * Material = MeshComp->GetMaterial(i);
			if (Material != nullptr)
			{
				UMaterialInstanceDynamic* MaterialInstance = MeshComp->CreateDynamicMaterialInstance(i, Material);
				if(MaterialInstance == nullptr)
				{
					continue;
				}

				const float NewMaterialValue =	FMath::Lerp(PropertyValue.X, PropertyValue.Y, LerpAlpha);

				MaterialInstance->SetScalarParameterValue(MaterialPropertyName, NewMaterialValue);
			}
		}
	}
	
}
