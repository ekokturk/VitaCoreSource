// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCCharacter.h"
#include "VCMinionCharacter.generated.h"

class USphereComponent;
class UVCShooterComponent;
class AVCHeroCharacter;
class UVCMeleeComponent;

UENUM(BlueprintType)
enum class EMinionAbilityTypes : uint8
{
	MeleeAttack UMETA(Input = "Ability1"),
	ProjectileAbility UMETA(Input = "Ability2"),
};


UCLASS()
class VITACORE_API AVCMinionCharacter : public AVCCharacter
{
	GENERATED_BODY()
	
// ================= MINION CHARACTER ========================
public:
	AVCMinionCharacter();

	virtual FString GetEntityName() const override;
	
protected:
	// Cue for InCombat state of the minion to apply visual change
	FGameplayTag Cue_InCombat;
	
	// Map that contains input abilities
	UPROPERTY(EditAnywhere, Category = "Config|Ability System", meta = (DisplayName = "Input Abilities"))
		TMap<EMinionAbilityTypes, TSubclassOf<UGameplayAbility>> AbilityMap;

	virtual void DoDeath(FDeathInfo DeathInfo = FDeathInfo()) override;

	UPROPERTY(VisibleAnywhere)
		UVCShooterComponent* ShooterComponent;
	
	UPROPERTY(EditAnywhere, Category = "Config|Minion Character")
		FName ShooterComponentSocketName;
	
	UPROPERTY(VisibleAnywhere)
		UVCMeleeComponent* LeftMeleeComponent;

	UPROPERTY(EditAnywhere, Category = "Config|Minion Character")
		FName LeftMeleeSocketName;
	
	UPROPERTY(VisibleAnywhere)
		UVCMeleeComponent* RightMeleeComponent;
	
	UPROPERTY(EditAnywhere, Category = "Config|Minion Character")
		FName RightMeleeSocketName;


protected:
	virtual void HandleCombatStateChange(const bool IsInCombat) override;
	
protected:
	virtual void InitializeAbilities() override;
	virtual void BeginPlay() override;
};
