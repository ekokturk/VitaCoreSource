// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCAbility_Projectile.h"
#include "VCAbility_ProjectileTargetAround.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCAbility_ProjectileTargetAround : public UVCAbility_Projectile
{
	GENERATED_BODY()
public:
	UVCAbility_ProjectileTargetAround();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Projectile Target Around Ability")
		float TargetCheckRadius;

	// This will determine the if actor can be effected based on it's affiliation
	UPROPERTY(EditAnywhere, Category = "Projectile Target Around Ability", meta = (Bitmask, BitmaskEnum = "EAffiliationType"))
		int TargetAffiliationMask;

	UPROPERTY()
		TArray<AActor*> TargetActors;

	UPROPERTY()
		int TargetIndex;
	
protected:
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnShooterStoppedShooting(UVCShooterComponent* ShooterComponent) override;
};
