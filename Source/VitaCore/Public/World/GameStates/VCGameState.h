// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VCGameTypes.h"
#include "VCGameState.generated.h"

class AVCGameMode;
class AVCPlayerTeam;
class AVCCharacter;
class AVCTeam;
class AVCPlayerController;
class AVCPlayerState;

class UVCHUD_Game;
class UVCUserWidget;

// Player state, Previous team, Current team
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPlayerTeamChanged, AVCPlayerState*, ETeamType, ETeamType)

/*
 * Base game state to be inherited by all gameplay related game state
 */

UCLASS()
class VITACORE_API AVCGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AVCGameState();

// ------ Player

	// Callback whenever a local player controller is created
	UFUNCTION() virtual void HandlePlayerLogin(AVCPlayerController* VCPlayerController);

	// Callback whenever a local player controller is destroyed
	UFUNCTION() virtual void HandlePlayerLogout(AVCPlayerController* VCPlayerController);
	
	// Get team actor reference
	UFUNCTION(BlueprintCallable) AVCTeam* GetTeam(ETeamType TeamType)  const;

	UFUNCTION() const TArray<TSubclassOf<AVCCharacter>>& GetPlayableCharacters() const { return PlayableCharacters; }

	// Get all current VC game states
	UFUNCTION() const TArray<AVCPlayerState*>& GetPlayers() const { return VCPlayerStates; }

	// Return all team actors that are available in the game
	UFUNCTION() TArray<AVCTeam*> GetAllTeams() const;

	// Delegate when player changes their team
	FOnPlayerTeamChanged OnPlayerTeamChanged;
	
// ----- Level Bracket

	// Get xp required to reach the next level bracket
	UFUNCTION() int GetLevelBracketXP(int LevelIndex) const;
	
	// Get total amount of xp required for a specific level
	UFUNCTION() int GetLevelBracketTotalXP(int LevelIndex) const;

	// Get xp required between two level brackets
	UFUNCTION() int GetLevelBracketXPInBetween(int FirstBracket, int SecondBracket);
	
	// Return the limit that level can reach
	UFUNCTION() int GetMaxLevel() const;

protected:

	// This will be called on both server and client before Begin Play is called for actors
	virtual void InitializeGameState();

	UFUNCTION() void AttachInitialWidgets(AVCPlayerController* VCPlayerController, int WidgetIndex);

protected:

	// List of characters that player can choose from
	UPROPERTY(EditAnywhere, Category = "Config")
		TArray<TSubclassOf<AVCCharacter>> PlayableCharacters;

	UPROPERTY(EditAnywhere, Category = "Config")
		TArray<TSubclassOf<UVCUserWidget>> InitialWidgets;
	
	// Cached Team Actor references
	TMap<ETeamType, AVCTeam*> TeamMap;

	// This will determine when experience trigger level up
	UPROPERTY(EditAnywhere, Category = "Config")
		TArray<int> LevelBrackets;

	// TODO Potentially use this instead of PlayerArray property of game state
	// Cached vitacore player states both in server and client
	UPROPERTY(Transient) TArray<AVCPlayerState*> VCPlayerStates;

	
protected:
	virtual void HandleBeginPlay() override;
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void BeginPlay() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
