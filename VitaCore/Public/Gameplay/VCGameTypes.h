// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "ScriptMacros.h"
#include "VCMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "VCGameTypes.generated.h"

// Delegate to notify XP Change (int DeltaXP, int TotalXP, int ProgressedBracketXP, int RequiredBracketXP)
DECLARE_MULTICAST_DELEGATE_FourParams(FOnXPChanged, int, int, int, int)

// Delegate to notify level change (Previous Level, Current Level)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int, int)

UENUM(BlueprintType)
enum class EEntityTypes : uint8
{
	None = 0,
	Tank,
	Archer,
	Engineer,
	Minion,
	FireTitan,
	IceTitan
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EAffiliationType : uint8
{
	NEUTRAL = 0,
	ALLY,
	ENEMY,
};

ENABLE_ENUM_FLAG_OPERATIONS(EAffiliationType)

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	NEUTRAL = 0 UMETA(DisplayName = "Neutral"),
	HOSTILE UMETA(DisplayName = "Hostile"),
	TEAM_VIOLET UMETA(DisplayName = "Violet"),
	TEAM_GOLD UMETA(DisplayName = "Gold")
};

UENUM(BlueprintType)
enum class ETerrainTypes: uint8
{
	GRASS = 0 UMETA(DisplayName = "Grass"),
	ROCK UMETA(DisplayName = "Rock"),
	SNOW UMETA(DisplayName = "Snow"),
};

UENUM(BlueprintType)
enum class ECoreGameModeTypes : uint8
{
	MAIN = 0,
	TUTORIAL,
	PRACTICE,
};

USTRUCT()
struct FHitInfo
{
	GENERATED_BODY()
	
	FHitInfo();
	FHitInfo(AActor * Source, AActor * Target, UPrimitiveComponent * SourceComp, UPrimitiveComponent * TargetComp);

	const FHitResult ConvertToHitResult() const;
	
	UPROPERTY() AActor* SourceActor;
	UPROPERTY() AActor* TargetActor;
	UPROPERTY() UPrimitiveComponent* SourceComponent;
	UPROPERTY() UPrimitiveComponent* TargetComponent;
	UPROPERTY() FVector ImpactLocation;

};

USTRUCT(BlueprintType)
struct FDeathInfo
{
	GENERATED_BODY()

	FDeathInfo()
		:IsDead(0),
		KillerActor(nullptr),
		KillerTeamType(ETeamType::NEUTRAL)
	{
	}

	FDeathInfo(AActor * Killer)
		:IsDead(1),
		KillerActor(Killer),
		KillerTeamType(ETeamType::NEUTRAL)
	{
		void SetKillerTeamType();
	}

	// State of death
	UPROPERTY() uint8 IsDead : 1;

	// If death state is true actor that is caused it, null otherwise
	UPROPERTY() AActor* KillerActor;

	// Team of the actor that has caused it
	UPROPERTY(NotReplicated) ETeamType KillerTeamType;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	void SetKillerTeamType();
	
};

template<>
struct TStructOpsTypeTraits<FDeathInfo> : public TStructOpsTypeTraitsBase2<FDeathInfo>
{
	enum
	{
		WithNetSerializer = true
	};
};


USTRUCT()
struct FSpawnAroundParams
{
	GENERATED_BODY()

	// Distance that actors spawned from
	UPROPERTY(EditAnywhere)
		float Distance;

	// Angle between each spawned actor (Disable if Max Angle is larger than 0)
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bShouldClampMaxAngle"))
		float AngleBetween;

	// Offset from the initial spawn angle
	UPROPERTY(EditAnywhere)
		float AngleOffset;

	UPROPERTY(EditAnywhere, meta =(InlineEditConditionToggle))
		bool bShouldClampMaxAngle;
	
	// Clamp max angle to be spawn around overrides AngleBetween
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bShouldClampMaxAngle"))
		float MaxAngle;
	
	FSpawnAroundParams();
};
