// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_VCSetFullBody.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UAnimNotify_VCSetFullBody : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
		bool ShouldDoFullBody;
};
