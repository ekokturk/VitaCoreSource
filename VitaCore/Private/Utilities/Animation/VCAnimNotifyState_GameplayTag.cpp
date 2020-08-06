// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "Animation/VCAnimNotifyState_GameplayTag.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"


void UVCAnimNotifyState_GameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (bShouldApplyOnAuthority == true && MeshComp->GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	IAbilitySystemInterface* OwnerAbilitySystem = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	if(OwnerAbilitySystem != nullptr)
	{
		OwnerAbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent();
	}

	if(OwnerAbilitySystemComponent != nullptr)
	{
		OwnerAbilitySystemComponent->AddLooseGameplayTags(GameplayTagContainer);
	}
}

void UVCAnimNotifyState_GameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (bShouldApplyOnAuthority == true && MeshComp->GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	IAbilitySystemInterface* OwnerAbilitySystem = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	if (OwnerAbilitySystem != nullptr)
	{
		OwnerAbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent();
	}
	if (OwnerAbilitySystemComponent != nullptr)
	{
		OwnerAbilitySystemComponent->RemoveLooseGameplayTags(GameplayTagContainer);
	}
}
