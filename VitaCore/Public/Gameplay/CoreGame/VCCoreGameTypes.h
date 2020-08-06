// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScriptMacros.h"
#include "VCGameTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "VCCoreGameTypes.generated.h"


// TODO Lower case conversion
UENUM(BlueprintType)
enum class EGameStages : uint8
{
	INITIAL = 0,
	SCOUTING,
	VITACORE,
	FIRETITAN,
	ICETITAN,
	FINAL
};




// TODO Make generic
UENUM(BlueprintType)
enum class ECoreActionTypes : uint8
{
	KILL = 0,
	KILLED_BY,
	SECURED,
	CAPTURED,
};


UENUM(BlueprintType)
enum class ECoreOutpostTypes : uint8
{
	Fortitude = 0,
	HEALTH,
	MOVEMENT,
	Mastery,
};


// Team about the state of the player team for core game mode
USTRUCT(BlueprintType)
struct FCoreTeamData
{

	GENERATED_BODY()

public:
	FCoreTeamData()
		:IsNetDirty(true),
		CoreDamage(0),
		MatchScore(0)
	{}

	float GetCoreDamage() const { return CoreDamage; }

	int GetMatchScore() const	{ return MatchScore;}

	void ResetCoreDamage()
	{
		if(CoreDamage != 0)
		{
			IsNetDirty = true;
		}
		CoreDamage = 0;
	}

	void AddCoreDamage(float InDamage)
	{
		if(InDamage > 0)
		{
			IsNetDirty = true;
		}
		
		CoreDamage += InDamage;
	}

	void AddMatchScore()
	{
		MatchScore++;
		IsNetDirty = true;
	}

	void Serialize(FArchive& Ar)
	{
		Ar << CoreDamage;
		Ar << MatchScore;
	}
	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Serialize(Ar);

		if(Ar.IsSaving()){
			IsNetDirty = false;
		}
		
		bOutSuccess = true;
		return true;
	}

	bool IsNetDirty;
	
private:
	UPROPERTY() float CoreDamage;
	UPROPERTY() int MatchScore;

};

template<>
struct TStructOpsTypeTraits<FCoreTeamData> : public TStructOpsTypeTraitsBase2<FCoreTeamData>
{
	enum
	{
		WithNetSerializer = true
	};
};


USTRUCT(BlueprintType)
struct FCoreTeamDataMap
{
	GENERATED_BODY()

	// Map that contains core game information about teams
	UPROPERTY() TMap<ETeamType, FCoreTeamData> Map;

	FCoreTeamData& operator[](ETeamType teamType)
	{
		return Map[teamType];
	}

	bool HasTeam(ETeamType teamType) const
	{
		return Map.Contains(teamType);
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* packageMap, bool& bOutSuccess)
	{
		if (Ar.IsSaving())
		{
			for(auto& MapPair : Map)
			{
				if(MapPair.Value.IsNetDirty)
				{
					uint8 TeamTypeAsByte = static_cast<uint8>(MapPair.Key);
					Ar << TeamTypeAsByte;
					MapPair.Value.Serialize(Ar);
				}
			}
		}
		else
		{
			UEnum* TeamEnumClass = StaticEnum<ETeamType>();
			while(Ar.AtEnd() == false)	// This check doesnt work it will break inside
			{
				uint8 TeamTypeAsByte;
				Ar << TeamTypeAsByte;

				// only accept non zero and available team types
				if(TeamTypeAsByte == 0 || TeamEnumClass->IsValidEnumValue(TeamTypeAsByte) == false)
				{
					break;
				}
				
				const ETeamType TeamType = static_cast<ETeamType>(TeamTypeAsByte);
				if(Map.Contains(TeamType) == false)
				{
					Map.Emplace(TeamType, FCoreTeamData());
				}

				Map[TeamType].Serialize(Ar);
			}
		}
		
		bOutSuccess = true;
		return true;
	}
	
};

template<>
struct TStructOpsTypeTraits<FCoreTeamDataMap> : public TStructOpsTypeTraitsBase2<FCoreTeamDataMap>
{
	enum
	{
		WithNetSerializer = true
	};
};


// (Previous Game Stage, Current Game Stage) 
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCoreGameStageChanged, EGameStages, EGameStages)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCoreTeamDataMapChanged, const FCoreTeamDataMap&)

enum class ETitanTypes : uint8;
class AVCTitanCharacter;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTitanStateChanged, ETitanTypes, AVCTitanCharacter*)