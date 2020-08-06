// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "AnimNotify_VCSelfGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotify_VCSelfGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if(MeshComp == nullptr)
	{
		return;
	}

	AActor * MyOwner = MeshComp->GetOwner();
	if( MyOwner != nullptr)
	{
		if(MyOwner->GetLocalRole() == ROLE_Authority)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MyOwner, EventTag, FGameplayEventData());
		}
	}
}