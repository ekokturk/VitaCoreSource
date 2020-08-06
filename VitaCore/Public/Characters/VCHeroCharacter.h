// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCCharacter.h"
#include "GameplayEffectTypes.h"
#include "VCHeroCharacter.generated.h"

class UVCEnergyAttributeSet;
class UGameplayAbility;
class UArrowComponent;
class AVCProjectile;
class UGameplayEffect;
class AVCPlayerTeam;
class AVCCoreGameMode;
class AVCCoreGameState;
class UStaticMeshComponent;
class UVCShooterComponent;

/*
 * Hero characters have a shooter component and five abilities
 * This is the base class for generic shooter player characters
 */


// Enum for Hero character ability input bindings
UENUM(BlueprintType)
enum class EHeroCharacterAbilityTypes : uint8
{
	LightAttack	UMETA(Input = "Ability1"),
	HeavyAttack	UMETA(Input = "Ability2"),
	UtilityAbility UMETA(Input = "Ability3"),
	ClassAbility UMETA(Input = "Ability4"),
	UltimateAbility	UMETA(Input = "Ability5"),
	Taunt	UMETA(Input = "Taunt")
};

UCLASS(hideCategories = (Variable))
class VITACORE_API AVCHeroCharacter : public AVCCharacter
{
	GENERATED_BODY()

// ================= HERO CHARACTER ===========================
public:
	AVCHeroCharacter();

	virtual FString GetEntityName() const override;

	// Get reference to energy attribute set
	UFUNCTION() UVCEnergyAttributeSet* GetEnergyAttributes() const;
	
protected:
	// Initialize base abilities
	UFUNCTION()
		virtual void InitializeAbilities() override;

protected:
	// Socket on the mesh that Weapon Mesh is attached to
	UPROPERTY(EditAnywhere, Category = "Config|Hero Character")
		FName WeaponSocketName;

	// Socket that defines end of the weapon mesh
	UPROPERTY(EditAnywhere, Category = "Config|Hero Character")
		FName MuzzleSocketName;

	// Map that contains input abilities
	UPROPERTY(EditAnywhere, Category = "Config|Ability System", meta=(DisplayName = "Input Abilities"))
		TMap<EHeroCharacterAbilityTypes, TSubclassOf<UGameplayAbility>> AbilityMap;

	FTimerHandle TimerHandle_Respawn;

	// ---- Components

	// Energy attributes to be used with abilities
	UPROPERTY() UVCEnergyAttributeSet* EnergyAttributeSet;

	// Main shooter component to fire projectiles
	UPROPERTY(EditAnywhere) UVCShooterComponent* ShooterComponent;

	// Mesh for the main weapon
	UPROPERTY(EditAnywhere) UStaticMeshComponent* WeaponMesh;
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};

