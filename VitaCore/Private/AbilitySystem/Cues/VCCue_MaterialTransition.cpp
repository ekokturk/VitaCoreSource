// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCue_MaterialTransition.h"

#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"
#include "WidgetComponent.h"

AVCCue_MaterialTransition::AVCCue_MaterialTransition() :
	MaterialPropertyName(NAME_None),
	bShouldUseFloatParameter(true),
	DesiredMaterialFloatParameter(0.0f),
	MeshComponentTag(NAME_None),
	InitialDelayOnActive(0.f),
	TransitionTimeTick(0.1f),
	TransitionTimeDuration(2.0f),
	bShouldRemoveWhenDesiredValueReached(true),
	OnRemovalMaterialAction(EVCCue_MaterialTransitionRemovalTypes::SetDesiredValue),
	TransitionAlpha(0.f)
{
	bAutoDestroyOnRemove = true;
	bUniqueInstancePerInstigator = true;
	bUniqueInstancePerSourceObject = true;
	bAllowMultipleOnActiveEvents = false;
	bAllowMultipleWhileActiveEvents = false;
}

void AVCCue_MaterialTransition::OnActiveTransition()
{
  	TransitionAlpha = FMath::Clamp(TransitionAlpha + TransitionTimeTick / TransitionTimeDuration, 0.f, 1.0f);
	for(auto MaterialInstance : DynamicMaterialInstances)
	{
		FMaterialParameterInfo Info(MaterialPropertyName);
		if(bShouldUseFloatParameter)
		{
			float CurrentValue;
			MaterialInstance->GetScalarParameterValue(Info, CurrentValue);
			const float NewMaterialValue = FMath::Lerp(CurrentValue, DesiredMaterialFloatParameter, TransitionAlpha);
			MaterialInstance->SetScalarParameterValue(MaterialPropertyName, NewMaterialValue);
		}
		else
		{
			FLinearColor CurrentValue;
			MaterialInstance->GetVectorParameterValue(Info, CurrentValue);
			const FLinearColor NewMaterialValue = FMath::Lerp(CurrentValue, DesiredMaterialVectorParameter, TransitionAlpha);
			MaterialInstance->SetVectorParameterValue(MaterialPropertyName, NewMaterialValue);
		}
	}

	if (TransitionAlpha >= 1.0f)
	{
		UWorld * World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().ClearTimer(TimerHandle_OnActiveTransition);
		}
		
		TransitionAlpha = 0.f;

		if(bShouldRemoveWhenDesiredValueReached)
		{
			GameplayCueFinishedCallback();
		}
	}

}
bool AVCCue_MaterialTransition::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// If property name is not defined stop finish cue
	if (MaterialPropertyName.IsValid() == false)
	{
		GameplayCueFinishedCallback();
		return false;
	}

	// Get all the mesh components of the actor
	TArray<UMeshComponent*> TargetMeshComponents;
	if(MeshComponentTag.IsValid())
	{
		TArray<UMeshComponent*>	AllMeshComponents;
		MyTarget->GetComponents<UMeshComponent>(AllMeshComponents);
		for (auto MeshComponent : AllMeshComponents)
		{
			TargetMeshComponents.Add(MeshComponent);
		}
	}
	else
	{
		MyTarget->GetComponents<UMeshComponent>(TargetMeshComponents);
	}

	// Cache created dynamic material instances from the mesh component materials
	for(auto MeshComponent : TargetMeshComponents)
	{
		// Do not change widget components
		if(MeshComponent->IsA(UWidgetComponent::StaticClass()))
		{
			continue;
		}

		// Cache created dynamic materials of the characters
		for (int i = 0; i < MeshComponent->GetNumMaterials(); i++)
		{
			UMaterialInterface * Material = MeshComponent->GetMaterial(i);
			if (Material != nullptr)
			{
				UMaterialInstanceDynamic* MaterialInstance = MeshComponent->CreateDynamicMaterialInstance(i, Material);
				if (MaterialInstance != nullptr)
				{
					DynamicMaterialInstances.Add(MaterialInstance);
					FMaterialParameterInfo Info(MaterialPropertyName);
					if (bShouldUseFloatParameter)
					{
						float CurrentValue;
						MaterialInstance->GetScalarParameterValue(Info, CurrentValue);
						InitialFloatParameters.Add(CurrentValue);
					}
					else
					{
						FLinearColor CurrentValue;
						MaterialInstance->GetVectorParameterValue(Info, CurrentValue);
						InitialVectorParameters.Add(CurrentValue);
					}
				}
			}
		}
	}
	
	// Start transition
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_OnActiveTransition);
		World->GetTimerManager().SetTimer(TimerHandle_OnActiveTransition, this,
			&AVCCue_MaterialTransition::OnActiveTransition, TransitionTimeTick, true, InitialDelayOnActive);
	}
	return true;
}

bool AVCCue_MaterialTransition::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{

	
	// Clear Timer
	if(UWorld * World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_OnActiveTransition);
		TransitionAlpha = 0.f;
	}
	// Set to desired value
	if(OnRemovalMaterialAction == EVCCue_MaterialTransitionRemovalTypes::SetDesiredValue)
	{
		for (auto MaterialInstance : DynamicMaterialInstances)
		{
			bShouldUseFloatParameter ?
			MaterialInstance->SetScalarParameterValue(MaterialPropertyName, DesiredMaterialFloatParameter):
			MaterialInstance->SetVectorParameterValue(MaterialPropertyName, DesiredMaterialVectorParameter);
		}
	}
	else if(OnRemovalMaterialAction == EVCCue_MaterialTransitionRemovalTypes::SetInitialValue)
	{
		for (int i=0; i < DynamicMaterialInstances.Num(); i++)
		{
			bShouldUseFloatParameter ?
				DynamicMaterialInstances[i]->SetScalarParameterValue(MaterialPropertyName, InitialFloatParameters[i]) :
				DynamicMaterialInstances[i]->SetVectorParameterValue(MaterialPropertyName, InitialVectorParameters[i]);
		}
	}
	DynamicMaterialInstances.Empty();
	return true;
}

