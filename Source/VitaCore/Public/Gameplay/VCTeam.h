// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "VCGameTypes.h"
#include "VCTeam.generated.h"

class AVCCharacter;
class AVCGameState;
class AVCSpawner;
class UGameplayEffect;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterJoinedTeam, AVCCharacter*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLeftTeam, AVCCharacter*)

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeamLevelChanged, int, int)

/*
 * This Team Actor is used 
 */

UCLASS(notplaceable, BlueprintType)
class VITACORE_API AVCTeam : public AActor
{
	GENERATED_BODY()
	
public:	
// ============== VC TEAM =========================
	AVCTeam();

	// Initialize replicated default team type
	UFUNCTION() void SetDefaultTeamType(ETeamType NewTeamType);

	// Initialize replicated default team name
	UFUNCTION() void SetDefaultTeamName(FName NewTeamName);

	// Initialize replicated default team color
	UFUNCTION() void SetDefaultTeamColor(FLinearColor NewTeamColor);

	// -------- Getters for team actor
	
	UFUNCTION()	ETeamType			GetTeamType() const			{ return TeamType; };
	UFUNCTION()	FLinearColor		GetTeamColor() const		{ return TeamColor; };
	UFUNCTION()	FName				GetTeamName() const			{ return TeamName; };
	UFUNCTION()	int					GetTeamLevel() const		{ return TeamLevel; }
	UFUNCTION()	int					GetTeamXP() const			{ return TeamXP; }
	UFUNCTION() UTexture2D*			GetTeamLogoImage() const	{ return TeamLogoTexture; }
	UFUNCTION() UMaterialInstance*	GetTeamLogoMaterial() const { return TeamLogoMaterialInstance; }
	UFUNCTION() float				GetRespawnTime() const		{ return CharacterRespawnTime;  }
	UFUNCTION() bool				IsPlayerTeam() const		{ return bIsPlayerTeam; }

	UFUNCTION() EAffiliationType	GetTeamAffiliation(ETeamType OtherTeamType) const;
	
	// ------- XP and Level

	UFUNCTION() void IncrementLevel();

	FOnTeamLevelChanged OnTeamLevelChanged;

	// Initialize team level and experience from default data
	// Default level will have control over the exp
	UFUNCTION() void InitLevelAndXP();
	
	// Increase team experience
	UFUNCTION() void ApplyXP(int XP);

	FOnXPChanged OnXPChanged;
	
// ============== TEAM FUNCTIONALITY =========================

	// Add a character to the team
	UFUNCTION() bool JoinTeam(AVCCharacter* JoiningCharacter);

	// Callback to be called when a character joins the team
	FOnCharacterJoinedTeam OnCharacterJoinedTeam;

	// Removes a character from the team
	UFUNCTION() bool LeaveTeam(AVCCharacter* LeavingCharacter);

	// Callback to be called when a character leaves the team
	FOnCharacterLeftTeam OnCharacterLeftTeam;

	// Return a reference to spawner that has the same type
	// SpawnerIndex returns specified index
	// If ShouldGetOnlyActive is true only active spawners will be returned
	UFUNCTION() AVCSpawner* GetSpawner(int SpawnerIndex = 0, bool ShouldGetOnlyActive = true) const;

	// Get the transform of a certain spawner, if not found spawn at team actor location
	UFUNCTION() FTransform GetSpawnTransform(int SpawnerIndex = 0) const;

	UFUNCTION() bool SpawnCharacter(AVCCharacter* Character) const;

	UFUNCTION() void ApplyTeamEffect(FGameplayEffectSpecHandle EffectSpecHandle);
	UFUNCTION() void RemoveTeamEffect(FGameplayEffectSpecHandle EffectSpecHandle);
	
protected:

	UFUNCTION() virtual void OnRep_TeamLevel(int PrevLevel);
	UFUNCTION() virtual void OnRep_TeamXP(int PrevXP);
	
protected:
	// Team type of the team
	UPROPERTY(EditAnywhere, Category = "Config")
		ETeamType TeamType;

	// Name of the team
	UPROPERTY(EditAnywhere, Category = "Config")
		FName TeamName;

	// If this is true, players can join this team
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bIsPlayerTeam;
	
	// If this is true, team can respawn characters
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bShouldRespawnMemberCharacters;
	
	// Affiliation of the team against other team types
	UPROPERTY(EditAnywhere, Category = "Config", 
		meta = (Bitmask, BitmaskEnum = "EAffiliationType", DisplayName = "Affiliation"))
		int AlliedAffiliationMask;
	
	// Color of the team
	UPROPERTY(EditAnywhere, Category = "Config")
		FLinearColor TeamColor;

	UPROPERTY(EditAnywhere, Category = "Config")
		UTexture2D* TeamLogoTexture;

	UPROPERTY(EditAnywhere, Category = "Config")
		UMaterialInstance* TeamLogoMaterialInstance;
	
	// Game level of the team
	UPROPERTY(ReplicatedUsing=OnRep_TeamLevel, EditAnywhere, Category = "Config")
		int TeamLevel;
	
	// Get team experience
	UPROPERTY(ReplicatedUsing = OnRep_TeamXP, EditAnywhere, Category = "Config")
		int TeamXP;

	UPROPERTY(EditAnywhere, Category = "Config")
		float CharacterRespawnTime;

	UPROPERTY() TArray<FGameplayEffectSpecHandle> InfiniteTeamEffects;
	
	// Currently available characters that are subscribed to this team
	UPROPERTY(Transient) TArray<AVCCharacter*> TeamMemberCharacters;
	
	// Team Spawners
	UPROPERTY(Transient) TArray<AVCSpawner*> TeamSpawners;
	
	// Cached game state reference
	UPROPERTY(Transient) AVCGameState* VCGameState;
	
protected:
	virtual void PostActorCreated() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
