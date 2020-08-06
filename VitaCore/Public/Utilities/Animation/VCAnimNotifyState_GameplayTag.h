// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "VCAnimNotifyState_GameplayTag.generated.h"

class UAbilitySystemComponent;

UCLASS()
class VITACORE_API UVCAnimNotifyState_GameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere,  Category = "GameplayTag Notify")
		FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(EditAnywhere, Category = "GameplayTag Notify")
		bool bShouldApplyOnAuthority;

	UPROPERTY()
		UAbilitySystemComponent* OwnerAbilitySystemComponent;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
