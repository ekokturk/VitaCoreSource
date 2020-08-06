// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_VCMaterialSetter.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class VITACORE_API UAnimNotifyState_VCMaterialSetter : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
		FName MaterialPropertyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
		FVector2D PropertyValue;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

private:

	UPROPERTY() float CurrentDuration;

	UPROPERTY() float MaxDuration;

	UPROPERTY() float AnimationRate;
	
};
