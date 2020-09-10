// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCoreGameState.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "VCPlayerState.h"
#include "Camera/CameraActor.h"
#include "CoreGame/VCCoreGameTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VCTitanCharacter.h"
#include "VCCharacter.h"

AVCCoreGameState::AVCCoreGameState()
	:RoundWinner(ETeamType::NEUTRAL),
	CurrentGameStage(EGameStages::INITIAL),
	FinalCameraTag("FinalCamera")
{
}

void AVCCoreGameState::BeginPlay()
{
	Super::BeginPlay();

	// Initialize final game camera actor
	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr->ActorHasTag(FinalCameraTag) == true)
		{
			FinalCameraActor = *ActorItr;
		}
	}
}

float AVCCoreGameState::GetCoreDamageRatio(ETeamType TeamType) const
{
	return TeamDataMap.Map.Contains(TeamType) ? TeamDataMap.Map[TeamType].GetCoreDamage() : 0;
}

int AVCCoreGameState::GetTeamMatchScore(ETeamType TeamType) const
{
	return TeamDataMap.Map.Contains(TeamType) ? TeamDataMap.Map[TeamType].GetMatchScore() : 0;
}

TArray<AVCCharacter*> AVCCoreGameState::GetAllPlayerCharacters() const
{
	TArray<AVCCharacter*> playerCharacters;
	for (const AVCPlayerState*  PlayerState : VCPlayerStates)
	{
		if(AVCCharacter * playerCharacter = Cast<AVCCharacter>(PlayerState->GetPawn()))
		{
			playerCharacters.Add(playerCharacter);
		}
	}
	return playerCharacters;
}



void AVCCoreGameState::InitializeGameState()
{
	Super::InitializeGameState();

	if(HasAuthority())
	{
		// Initialize team data for existing teams
		for (const auto& TeamPair : TeamMap)
		{
			TeamDataMap.Map.Emplace(TeamPair.Key) = FCoreTeamData();
		}
	}
}

void AVCCoreGameState::OnRep_CurrentGameStage(EGameStages PrevStage)
{
	BP_OnRep_CurrentGameStage();
	OnCoreGameStageChanged.Broadcast(PrevStage, CurrentGameStage);
}

void AVCCoreGameState::OnRep_TeamDataMap()
{
	OnCoreTeamDataChanged.Broadcast(TeamDataMap);
}

void AVCCoreGameState::OnRep_IceTitan()
{
	OnTitanStateChanged.Broadcast(ETitanTypes::Ice, IceTitan);
}

void AVCCoreGameState::OnRep_FireTitan()
{
	OnTitanStateChanged.Broadcast(ETitanTypes::Fire, FireTitan);
}


void AVCCoreGameState::SendMessage_VitaCoreCapture_Implementation(ETeamType TeamType)
{
	BP_OnSendMessage_VitaCoreCapture(TeamType);
}

void AVCCoreGameState::SendMessage_CharacterKill_Implementation(AActor* KillerActor, AActor* DefeatedActor)
{
	BP_SendMessage_CharacterKill(KillerActor, DefeatedActor);
}

void AVCCoreGameState::DoMatchFinal()
{
	BP_DoMatchFinal();
}


void AVCCoreGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVCCoreGameState, MatchWinner);
	DOREPLIFETIME(AVCCoreGameState, RoundWinner);
	DOREPLIFETIME(AVCCoreGameState, CurrentGameStage);
	DOREPLIFETIME(AVCCoreGameState, CoreCaptureDamage);
	
	DOREPLIFETIME(AVCCoreGameState, MatchStartCounter);
	DOREPLIFETIME(AVCCoreGameState, VitaCoreActivationCounter)

	DOREPLIFETIME(AVCCoreGameState, IceTitan);
	DOREPLIFETIME(AVCCoreGameState, FireTitan);

	DOREPLIFETIME(AVCCoreGameState, TeamDataMap)
}




