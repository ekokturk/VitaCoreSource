// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCCharacter.h"
#include "GameplayTagContainer.h"
#include "VCTitanCharacter.generated.h"

class UVCMeleeComponent;
class USphereComponent;
class AVCHeroCharacter;
class UVCShooterComponent;

UENUM(BlueprintType)
enum class ETitanAbilityTypes : uint8
{
	MeleeAttack	UMETA(Input = "Ability1"),
	GlobalAbility UMETA(Input = "Ability2"),
	ProjectileAbility UMETA(Input = "Ability3"),
};

UENUM(BlueprintType)
enum class ETitanTypes : uint8
{
	Ice = 0,
	Fire,
};


UCLASS()
class VITACORE_API AVCTitanCharacter : public AVCCharacter
{
	GENERATED_BODY()

public:
	AVCTitanCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		ETitanTypes TitanType;

	virtual FString GetEntityName() const override;

	
protected:

	// Map that contains input abilities
	UPROPERTY(EditAnywhere, Category = "Config|Abilities", meta = (DisplayName = "Input Abilities"))
		TMap<ETitanAbilityTypes, TSubclassOf<UGameplayAbility>> AbilityMap;

	
	UPROPERTY(VisibleAnywhere)
		UVCMeleeComponent * LeftMeleeComponent;

	UPROPERTY(EditAnywhere, Category = "Config")
		FName LeftMeleeSocketName;

	UPROPERTY(VisibleAnywhere)
		UVCMeleeComponent* RightMeleeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName RightMeleeSocketName;

	UPROPERTY(VisibleAnywhere)
		UVCShooterComponent* ShooterComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ShooterComponentSocketName;
	
protected:
	virtual void InitializeAbilities() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void DoDeath(FDeathInfo DeathInfo) override;



};
