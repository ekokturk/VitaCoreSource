// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayAbilityTypes.h"
#include "AnimNotify_VCSelfGameplayEvent.generated.h"


/**
 * 
 */
UCLASS()
class VITACORE_API UAnimNotify_VCSelfGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEvent")
		FGameplayTag EventTag;
	
};
