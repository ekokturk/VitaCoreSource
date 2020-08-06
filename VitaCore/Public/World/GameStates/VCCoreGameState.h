// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameState.h"
#include "VCGameTypes.h"
#include "VCCoreGameTypes.h"
#include "VCCoreGameState.generated.h"

class AVCPlayerTeam;
class AVCCoreGameMode;
class AVCCharacter;
class AAVCVitaCoreShrine;
class AVCTitanCharacter;
class ACameraActor;

UCLASS()
class VITACORE_API AVCCoreGameState : public AVCGameState
{
	GENERATED_BODY()

public:
	AVCCoreGameState();

	UFUNCTION() void DoMatchFinal();

	// If true, match is started
	UFUNCTION() bool IsMatchStarted() const { return bIsMatchStarted; };

	// Return the countdown timer to begin the match
	UFUNCTION() float GetMatchStartTimer() const { return MatchStartCounter; };

	// Return the current game stage of the match
	UFUNCTION() EGameStages GetCurrentGameStage() const { return CurrentGameStage; };

	// Return the camera reference to transition when the game ends
	UFUNCTION() ACameraActor* GetFinalCameraActor() const { return FinalCameraActor; };

	// Return the time until vitacore activation
	UFUNCTION() float GetVitaCoreActivationTimer() const { return VitaCoreActivationCounter; };

	// Return winner team type
	UFUNCTION() ETeamType GetMatchWinner() const { return MatchWinner; };
	
	// Return vitacore shrine damage ratio for a specific team
	UFUNCTION() float GetCoreDamageRatio(ETeamType TeamType) const;

	// Return match score for a specific team
	UFUNCTION() int GetTeamMatchScore(ETeamType TeamType) const;

	UFUNCTION() TArray<AVCCharacter*> GetAllPlayerCharacters() const;

	// Custom map that will contain data for all teams
	UPROPERTY(ReplicatedUsing=OnRep_TeamDataMap) FCoreTeamDataMap TeamDataMap;
	
	FOnCoreGameStageChanged OnCoreGameStageChanged;

	FOnCoreTeamDataMapChanged OnCoreTeamDataChanged;
	
protected:

	virtual void InitializeGameState() override;
	
	// Callback that will be called on client when game stage changes
	UFUNCTION() void OnRep_CurrentGameStage(EGameStages PrevStage);

	UFUNCTION() void OnRep_TeamDataMap();

	UFUNCTION() void OnRep_IceTitan();

	UFUNCTION() void OnRep_FireTitan();

	// Message to notify players that vitacore shrine is captured
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void SendMessage_VitaCoreCapture(ETeamType TeamType);

	// Message to notify players that a character is killed
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void SendMessage_CharacterKill(AActor* KillerActor, AActor* DefeatedActor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnRep_CurrentGameStage"))
		void BP_OnRep_CurrentGameStage();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BP_OnRep_RoundWinner"))
		void BP_OnRep_RoundWinner();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMatchFinal"))
		void BP_DoMatchFinal();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSendMessage_VitaCoreCapture"))
		void BP_OnSendMessage_VitaCoreCapture(ETeamType TeamType);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSendMessage_CharacterKill"))
		void BP_SendMessage_CharacterKill(AActor* KillerActor, AActor* DefeatedActor);

private:

	// If true, match is started
	UPROPERTY(Replicated) bool bIsMatchStarted;

	// Total damage to capture vitacore shrine
	UPROPERTY(Replicated) float CoreCaptureDamage;

	// Countdown timer to activate vitacore shrine
	UPROPERTY(Replicated) float VitaCoreActivationCounter;

	// Countdown timer to begin the match
	UPROPERTY(Replicated) int MatchStartCounter;
	
	// Winner team of the match
	UPROPERTY(Replicated)
		ETeamType MatchWinner;

	// Winner of the last round (Team that captured the vitacore shrine)
	UPROPERTY(ReplicatedUsing = BP_OnRep_RoundWinner)
		ETeamType RoundWinner;

	// Current stage of the match
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameStage)
		EGameStages CurrentGameStage;
	
	// Tag that will be searched for the camera transition when match ends
	UPROPERTY(EditAnywhere, Category = "Config")
		FName FinalCameraTag;
	
private:
	
	// Final camera that will be possessed when the match finishes
	UPROPERTY(Transient) ACameraActor * FinalCameraActor;

	//TODO Set these to private and have multicast events for titans
public:
	// Reference to the FireTitan
	UPROPERTY(ReplicatedUsing=OnRep_FireTitan) AVCTitanCharacter* FireTitan;

	// Reference to the Ice Titan
	UPROPERTY(ReplicatedUsing=OnRep_IceTitan) AVCTitanCharacter* IceTitan;

	FOnTitanStateChanged OnTitanStateChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	friend class AVCCoreGameMode;
};
