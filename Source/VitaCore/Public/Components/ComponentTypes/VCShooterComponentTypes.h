// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "VCGameTypes.h"
#include "VCShooterComponentTypes.generated.h"

class AVCLaser;
class AVCProjectile;

// Determines type of projectile shooting
UENUM()
enum class EProjectileShootTypes : uint8
{
	DEFAULT = 0,
	SPREAD,
	RANDOM
};

// Determines how shooter component will be updated
UENUM()
enum class EShooterComponentDirectionUpdateType : uint8
{
	NO_UPDATE = 0,
	UPDATE,
};

// Used to contain data of delayed operations
USTRUCT()
struct FShooterTimerProps
{
	GENERATED_BODY()

	FTimerHandle TimerHandle;
	float PassedTimeCounter;
	int CurrentTimeTick;

	FShooterTimerProps();

	bool IsValid() const
	{
		return TimerHandle.IsValid();
	}

	int GetTimeTick() const
	{
		return CurrentTimeTick;
	}

	FTimerHandle& GetHandle()
	{
		return TimerHandle;
	}
	
	void UpdateTimeCounter(float DeltaDelay)
	{
		CurrentTimeTick++;
		PassedTimeCounter += DeltaDelay;
	}

	bool ShouldTimerEnd(float ExpectedTimerCount) const
	{
		return ExpectedTimerCount < PassedTimeCounter;
	}

	void Clear(FTimerManager& TimerManager);
};

// Properties that projectile actor will use for spawning
USTRUCT(BlueprintType)
struct FProjectileShootParams
{
	GENERATED_BODY()

		FProjectileShootParams();

	// Amount of projectiles to be spawned
	UPROPERTY(EditAnywhere)
		int Count;

	// Delay between each spawned projectile
	UPROPERTY(EditAnywhere)
		float DelayBetweenShots;

	// Initial speed of the projectiles
	UPROPERTY(EditAnywhere)
		float InitialSpeed;

	// Type to determine how the projectiles will be spawned
	UPROPERTY(EditAnywhere)
		EProjectileShootTypes ProjectileShootType;

	// Parameters to be used if selected shoot type is spread
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ProjectileShootType == EProjectileShootTypes::SPREAD"))
		FSpawnAroundParams SpreadParams;

	// Determines how much randomization will occur in vertical axis if selected shoot type is random
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ProjectileShootType == EProjectileShootTypes::RANDOM"))
		FVector2D VerticalRandomAngles;

	// Determines how much randomization will occur in horizontal axis if selected shoot type is random
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ProjectileShootType == EProjectileShootTypes::RANDOM"))
		FVector2D HorizontalRandomAngles;

	// Effect that will be applied to projectile
	UPROPERTY()
		FGameplayEffectSpecHandle GameplayEffectHandle;

	// Timer properties that will determine the delay of each projectile and end of operation
	UPROPERTY()
		FShooterTimerProps TimerProps;

	// Target to shoot (If available, it will overwrite projectile orientation to face the target)
	UPROPERTY()
		AActor* Target;

};

// Properties that laser actor will use for spawning
USTRUCT(BlueprintType)
struct FLaserShootParams
{
	GENERATED_BODY()

	FLaserShootParams();

	// Handle for the effect that will be applied to allies
	UPROPERTY()
		FGameplayEffectSpecHandle AllyEffectHandle;

	// Handle for the effect that will be applied to enemies
	UPROPERTY()
		FGameplayEffectSpecHandle EnemyEffectHandle;
};



