// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbilitySystemInterface.h"
#include "VCActor.h"
#include "VCHealthAttributeSet.h"
#include "VCVitaCoreShrine.generated.h"

class AVCTeam;
class UGameplayEffect;
class UParticleSystemComponent;
class UVCHealthAttributeSet;
class UCapsuleComponent;
class UVCEffectComponent;
class UVCAbilitySystemComponent;

UCLASS()
class VITACORE_API AVCVitaCoreShrine : public AVCActor, public IVCAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	AVCVitaCoreShrine();

	// Activate or deactivate vitacore shield 
	UFUNCTION() void SetShieldActive(bool IsActive);

	// This will set hitpoints of the vitacore
	UFUNCTION() void SetHitPoints(float HitPoints)  const;

	// To be called when  game ends
	UFUNCTION() void DoCapture(AVCTeam* CapturedTeam);
	
	// If true, vitacore shrine is active
	UFUNCTION() bool IsShieldActive() const { return bIsShieldActive; }

	UFUNCTION() UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Get level of effects that will be used by this character
	UFUNCTION() int GetAbilityLevel() const override { return 0; };

	// Get On damage delegate from health attribute set
	FOnDamage& GetOnDamageDelegate() const { return HealthAttributeSet->OnDamage; };

protected:

	// Internal notification for damage
	UFUNCTION() void HandleDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth);

	// Client-side callback to be called when active state is changed
	UFUNCTION() void OnRep_IsShieldActive(bool IsShieldActivePrev);

	// Callback to be triggered when shield is activated both on server and client
	UFUNCTION() void HandleShieldActivated();

	// Callback to be triggered when shield is deactivated both on server and client
	UFUNCTION() void HandleShieldDeactivated();

	// Expand shield on tick
	UFUNCTION() void ExpandShield();
	
protected:

	// Determines VitaCore shield is active and effect is applying
	UPROPERTY(ReplicatedUsing = OnRep_IsShieldActive, EditAnywhere, Category = "Config")
		bool bIsShieldActive;

	// Maximum radius shield will extend to
	UPROPERTY(EditAnywhere, Category = "Config") float ShieldMaxRadius;

	// How often expansion step should be applied
	UPROPERTY(EditAnywhere, Category = "Config") float ShieldExpansionRate;

	// How much change in radius must be applied to shield each expansion step
	UPROPERTY(EditAnywhere, Category = "Config") float ShieldExpansionAmount;

	
	UPROPERTY() float ShieldCurrentRadius;
	
	FTimerHandle TimerHandle_ShieldExpansion;

	FGameplayTag Tag_Immune;

	// ----- Components -----
	
	UPROPERTY(VisibleAnywhere) UVCEffectComponent* ShieldEffectComponent;
	UPROPERTY(VisibleAnywhere) UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere) USkeletalMeshComponent* ShrineMesh;
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* ShieldMesh;
	UPROPERTY(VisibleAnywhere) UParticleSystemComponent* ShieldParticle;
	UPROPERTY(VisibleAnywhere) UParticleSystemComponent* ActiveShieldParticle;
	UPROPERTY() UVCHealthAttributeSet* HealthAttributeSet;
	UPROPERTY() UVCAbilitySystemComponent* AbilitySystemComponent;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
