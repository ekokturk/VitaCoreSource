// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/VCOverlapComponent.h"
#include "GameplayEffectTypes.h"
#include "VCAbilitySystemInterface.h"
#include "VCEffectComponent.generated.h"

class UGameplayEffect;

/**
 * This component can be attach to an ability actor and apply effect to overlapped characters
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VITACORE_API UVCEffectComponent : public UVCOverlapComponent
{
	GENERATED_BODY()

// ================== EFFECT COMPONENT ======================
public:
	UVCEffectComponent();

	// Return level of the effects that will be applied to target
	UFUNCTION()
		int GetEffectLevel() const;

protected:
	
	// Cached ability system interface for component owner
	UPROPERTY()
		TScriptInterface<IVCAbilitySystemInterface> OwnerAbilitySystem;

	virtual void HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation, const FHitInfo HitInfo ) override;
	virtual void HandleOverlapActorRemoved(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation) override;

// ================== EFFECT COMPONENT CONFIG ======================
protected:
	// Effect to be applied to enemies
	UPROPERTY(EditAnywhere, Category = "Effect Component")
		TSubclassOf<UGameplayEffect> Effect;

	// If this is true effect will be removed when actor is no longer overlapping
	UPROPERTY(EditAnywhere, Category = "Effect Component")
		bool bRemoveEffectOnOverlapEnds;
	
	// If this is true, component use owner level for effects rather than EffectLevel property
	UPROPERTY(EditAnywhere, Category = "Effect Component")
		bool bShouldUseOwnerAbilityLevel;
	
	// This will be used for effect level if bShouldUseOwnerAbilityLevel is false
	UPROPERTY(EditAnywhere, Category = "Effect Component", meta = (EditCondition = "!bShouldUseOwnerAbilityLevel"))
		int EffectLevel;
	
// ================== ACTOR COMPONENT ======================
protected:
	virtual void BeginPlay() override;
	
};
