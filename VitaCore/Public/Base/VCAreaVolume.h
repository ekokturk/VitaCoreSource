// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/VCActor.h"
#include "GameplayEffectTypes.h"
#include "VCAbilitySystemInterface.h"
#include "VCAreaVolume.generated.h"


class USphereComponent;
class UDecalComponent;
class UAbilitySystemComponent;
class UGameplayEffect;
class UParticleSystemComponent;
class AVCCharacter;

/*
 * Area volume is an AOE effect area that is placed on the map
 * It will get properties from the instigator (if exits) to determine who to apply the effect
 */
UCLASS()
class VITACORE_API AVCAreaVolume : public AVCActor, public IVCAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AVCAreaVolume();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };

	virtual int GetAbilityLevel() const override { return 0; };

protected:
	// Activate effect so it could be applied to designated characters
	UFUNCTION() void ActivateAreaEffect();
	
	// Set team color of the character that spawned the area volume
	UFUNCTION() void SetInstigatorTeamColor()  const;

	// Callback when an actor enters area volume
	UFUNCTION() void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	// Callback when an actor exits area volume
	UFUNCTION() void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	// Cosmetic blueprint function to notify effect activation
	UFUNCTION(BlueprintImplementableEvent, Category = "Config", meta = (DisplayName = "OnEffectActivation"))
		void BP_OnEffectActivation();
	
protected:
	// Effect that will be applied to enemies
	UPROPERTY(EditAnywhere, Category = "Config|AreaVolume")
		TSubclassOf<UGameplayEffect> EnemyEffect;

	// Effect that will be applied to allies
	UPROPERTY(EditAnywhere, Category = "Config|AreaVolume")
		TSubclassOf<UGameplayEffect> AllyEffect;

	// If true, effect will be removed on exit
	UPROPERTY(EditAnywhere, Category = "Config|AreaVolume")
		bool bIsEffectRemovedOnExit;

	// If this is larger than 0, effect will be activated based on a delay
	UPROPERTY(EditAnywhere, Category = "Config|AreaVolume")
		float EffectInitialDelay;

	// Color parameter that will be used to change all colors to team color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|AreaVolume")
		FName TeamColorParameterName;
	
	FTimerHandle TimerHandle_EffectActivation;

	// Creator character of area volume
	UPROPERTY(Transient) AVCCharacter* InstigatorCharacter;

	UPROPERTY() UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "AreaVolume")
		USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "AreaVolume")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "AreaVolume")
		UDecalComponent* AreaDecal;

	UPROPERTY(VisibleAnywhere, Category = "AreaVolume")
		UParticleSystemComponent* ParticleSystemComponent;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void OnRep_Instigator() override;
};
