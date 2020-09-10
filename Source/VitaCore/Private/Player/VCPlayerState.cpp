// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCPlayerState.h"
#include "UnrealNetwork.h"


AVCPlayerState::AVCPlayerState()
	:TeamType(ETeamType::NEUTRAL),
	DefaultPlayerName(TEXT("NoName"))
{
	bReplicates = true;
}

void AVCPlayerState::SetTeamType(ETeamType NewTeamType)
{
	const ETeamType PrevTeam = TeamType;
	TeamType = NewTeamType;
	OnTeamChanged.Broadcast(PrevTeam, TeamType);
}

void AVCPlayerState::OnRep_TeamType(ETeamType PrevTeam)
{
	OnTeamChanged.Broadcast(PrevTeam,TeamType);
}

void AVCPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetPlayerNameInternal(DefaultPlayerName);
}

// Replicate properties
void AVCPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVCPlayerState, TeamType);
}