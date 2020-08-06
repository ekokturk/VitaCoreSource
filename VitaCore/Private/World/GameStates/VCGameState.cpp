// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCGameState.h"

#include "EngineUtils.h"
#include "UnrealNetwork.h"
#include "VCHUD_CharacterSelection.h"
#include "VCPlayerController.h"
#include "VCPlayerState.h"
#include "VCTeam.h"
#include "VitaCoreLog.h"
#include "VCPlayerState.h"
#include "VCHUD_TeamSelection.h"

AVCGameState::AVCGameState()
{
	// TODO Initialize from assets
	LevelBrackets = { 500, 1000, 2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000 };
}

void AVCGameState::HandlePlayerLogin(AVCPlayerController* VCPlayerController)
{
	// Recursively attach widgets
	AttachInitialWidgets(VCPlayerController, 0);

}

void AVCGameState::HandlePlayerLogout(AVCPlayerController* VCPlayerController)
{
	
}

void AVCGameState::InitializeGameState()
{
	for (TActorIterator<AVCTeam> TeamItr(GetWorld()); TeamItr; ++TeamItr)
	{
		TeamMap.FindOrAdd(TeamItr->GetTeamType(), *TeamItr);
	}
	
	if(TeamMap.Num() == 0)
	{
		VC_LOG(Error, "No team actors found!")
	}
}

void AVCGameState::AttachInitialWidgets(AVCPlayerController* VCPlayerController, int WidgetIndex)
{
	if(VCPlayerController && InitialWidgets.IsValidIndex(WidgetIndex))
	{
		
		UVCUserWidget* Widget = CreateWidget<UVCUserWidget>(VCPlayerController, InitialWidgets[WidgetIndex]);
		check(Widget)
		Widget->AddToViewport();
		// Set callback for next widget
		if(WidgetIndex + 1 < InitialWidgets.Num())
		{
			Widget->OnWidgetDeactivated.AddWeakLambda(this, [=](const UVCUserWidget* UserWidget)
			{
				AttachInitialWidgets(VCPlayerController, WidgetIndex + 1);
			});
		}
	}
}


void AVCGameState::HandleBeginPlay()
{
	InitializeGameState();
	Super::HandleBeginPlay();
}

void AVCGameState::OnRep_ReplicatedHasBegunPlay()
{
	InitializeGameState();
	Super::OnRep_ReplicatedHasBegunPlay();
}

void AVCGameState::BeginPlay()
{
	Super::BeginPlay();
	// Cache teams in the world for referencing

}

void AVCGameState::AddPlayerState(APlayerState* PlayerState)
{
	if (PlayerState->IsInactive() == false)
	{
		PlayerArray.AddUnique(PlayerState);
		AVCPlayerState* VCPlayerState = Cast<AVCPlayerState>(PlayerState);
		check(VCPlayerState)
		VCPlayerState->OnTeamChanged.AddWeakLambda(this, 
			[=, &VCPlayerState](ETeamType PrevTeam, ETeamType CurrentTeam){
				OnPlayerTeamChanged.Broadcast(VCPlayerState, PrevTeam, CurrentTeam);
			});
		VCPlayerStates.AddUnique(VCPlayerState);
	}
}

void AVCGameState::RemovePlayerState(APlayerState* PlayerState)
{
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (PlayerArray[i] == PlayerState)
		{
			PlayerArray.RemoveAt(i, 1);
			check(VCPlayerStates[i] == PlayerState)
			VCPlayerStates[i]->OnTeamChanged.RemoveAll(this);
			VCPlayerStates.RemoveAt(i, 1);
			return;
		}
	}
}

AVCTeam * AVCGameState::GetTeam(ETeamType TeamType) const
{
	return TeamMap.Contains(TeamType) ? TeamMap[TeamType] : nullptr;
}

TArray<AVCTeam*> AVCGameState::GetAllTeams() const
{
	TArray<AVCTeam*> TeamArray;
	TeamMap.GenerateValueArray(TeamArray);
	return TeamArray;
}

int AVCGameState::GetLevelBracketXP(int LevelIndex) const
{
	return GetLevelBracketTotalXP(LevelIndex) - GetLevelBracketTotalXP(LevelIndex - 1);
}

int AVCGameState::GetLevelBracketTotalXP(int LevelIndex) const
{
	if(LevelBrackets.IsValidIndex(LevelIndex))
	{
		return LevelBrackets[LevelIndex];
	}

	VC_LOG(Error, "Level Bracket is not found for the specified level in the Game State! Closest bracket value returned...")
	
	if (LevelIndex >= LevelBrackets.Num())
	{
		return LevelBrackets.Last();
	}

	return 0;
}

int AVCGameState::GetLevelBracketXPInBetween(int FirstBracket, int SecondBracket)
{
	return FMath::Abs(GetLevelBracketTotalXP(FirstBracket) - GetLevelBracketTotalXP(SecondBracket));
}

int AVCGameState::GetMaxLevel() const
{
	return FMath::Clamp(LevelBrackets.Num() - 1, 0, LevelBrackets.Num() - 1);
}


void AVCGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
