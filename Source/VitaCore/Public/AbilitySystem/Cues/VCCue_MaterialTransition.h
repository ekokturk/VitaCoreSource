// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameplayCueNotifyActor.h"
#include "VCCue_MaterialTransition.generated.h"

class UMeshComponent;

/**
 * This cue is to change a material parameter of actor mesh components (float/vector) with a timed transition
 * Mesh components can be found using tags otherwise all the components will be used (Excluding widget component)
 */

UENUM()
enum class EVCCue_MaterialTransitionRemovalTypes : uint8
{
	NoChange,
	SetDesiredValue,
	SetInitialValue
};

UCLASS()
class VITACORE_API AVCCue_MaterialTransition : public AVCGameplayCueNotifyActor
{
	GENERATED_BODY()

public:
	AVCCue_MaterialTransition();

protected:
	// Timer method to apply transition based on the selected interval
	UFUNCTION(BlueprintCallable)
		void OnActiveTransition();

	// Timer handle for material transition
	FTimerHandle TimerHandle_OnActiveTransition;
	
protected:
	// Property that will be changed on material
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		FName MaterialPropertyName;
	
	// If true, float parameter for the material be used instead of vector parameter
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition", meta = (InlineEditConditionToggle))
		bool bShouldUseFloatParameter;
	
	// Desired float value to set material parameter to
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition", meta = (EditCondition = "bShouldUseFloatParameter"))
		float DesiredMaterialFloatParameter;

	// Desired vector value to set material parameter to
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition", meta = (EditCondition = "!bShouldUseFloatParameter"))
		FLinearColor DesiredMaterialVectorParameter;

	// If this value is valid, only mesh components with this tag will transition
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		FName MeshComponentTag;
	
	// Whenever cue is activated, this delay will be waited before material transition occurs
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		float InitialDelayOnActive;

	// This value is the delay between each material transition step
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		float TransitionTimeTick;

	// Total expected duration of the time to be
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		float TransitionTimeDuration;
	
	// If true, desired parameter values will set
	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		bool bShouldRemoveWhenDesiredValueReached;

	UPROPERTY(EditAnywhere, Category = "Cue - MaterialTransition")
		EVCCue_MaterialTransitionRemovalTypes OnRemovalMaterialAction;

	// Alpha value of transition based on the current value for lerping
	UPROPERTY()
		float TransitionAlpha;
	
	// Cached material instances
	UPROPERTY()
		TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;

	UPROPERTY()
		TArray<FLinearColor> InitialVectorParameters;

	UPROPERTY()
		TArray<float> InitialFloatParameters;
	
protected:
	
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
