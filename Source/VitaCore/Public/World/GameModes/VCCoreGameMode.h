// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameMode.h"
#include "VCCoreGameMode.generated.h"


enum class EGameStages : unsigned char;
class UVCHUD_CharacterSelection;
class AVCHeroCharacter;
class AVCCoreGameState;
class AVCPlayerTeam;
class AVCSpawner;
class AVCVitaCoreShrine;
class AVCPropController;

/*
 * Core game mode is the original game mode of VitaCore
 * Aim is to capture vitacore shrine while fighting with AI and controlling the map
 */

UCLASS()
class VITACORE_API AVCCoreGameMode : public AVCGameMode
{
	GENERATED_BODY()

public:
	AVCCoreGameMode();

	// Start match
	UFUNCTION() void StartVitaCoreMatch();

	// Apply Damage to vitacore based on player team
	UFUNCTION() void AddCoreDamage(ETeamType DamagerTeamType, float Damage);

	// Skip to the next game stage
	UFUNCTION() void GoNextGameStage();

	// Change game stage in the game state
	UFUNCTION() void SetGameCurrentStage(EGameStages NewGameStage) const;

	// Reset Core damage
	UFUNCTION() void ResetCore()  const;
	
	// Check if match is won, if so set winner in the game state
	UFUNCTION() bool IsMatchWon() const;

private:
	
	UFUNCTION() void MatchStartCounterTimer();

	// Countdown for vitacore activation
	UFUNCTION() void CountVitaCoreTimer(float DeltaSeconds);

	// Initialize helper actors that will be used by game mode
	UFUNCTION() void InitializeActors();

	// Check if vitacore is captured if so add a match score to the team that captured it
	UFUNCTION() bool VerifyCoreCapture(ETeamType PlayerTeam) const;

	// Spawn a titan at at specific location
	UFUNCTION() void SpawnTitan();

	UFUNCTION() void HandleCoreDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth);
	
private:

	// Match Score required for a player team to win
	UPROPERTY(EditAnywhere, Category = "Config")
		int ScoreToWin;

	// Time that VitaCore Activate
	UPROPERTY(EditAnywhere, Category = "Config")
		float VitaCoreActivationTimer;

	// Check if fire titan is spawned
	UPROPERTY() bool bIsFireTitanSpawned;

	// Check if Ice titan is spawn
	UPROPERTY() bool bIsIceTitanSpawned;
	
	// Amount of damage core requires to be captured
	UPROPERTY(EditAnywhere, Category = "Config")
		float CoreCaptureDamage;

	// Seconds to wait before match starts
	UPROPERTY(EditAnywhere, Category = "Config")
		int MatchStartWaitTime;

	UPROPERTY(EditAnywhere, Category = "Config")
		int MatchStartFinalCountdown;

	UPROPERTY(EditAnywhere, Category = "Config")
		float CoreCaptureMultiplier;

	FTimerHandle TimerHandle_MatchStartCounter;
	
private:

	// Vitacore shrine which is the main objective
	UPROPERTY(Transient) AVCVitaCoreShrine* VitaCoreShrine;
	
	// Fire Titan that will spawn after first VitaCore stage
	UPROPERTY(Transient) AVCSpawner* FireTitanSpawner;

	// Ice Titan that will spawn after second VitaCore stage
	UPROPERTY(Transient) AVCSpawner* IceTitanSpawner;

	// Actor which manages assets based on the game state
	UPROPERTY(Transient) AVCPropController * PropController;

	UPROPERTY(Transient) AVCCoreGameState * CoreGameState;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
