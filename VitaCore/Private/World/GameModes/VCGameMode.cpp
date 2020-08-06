// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCGameMode.h"

#include "Editor.h"
#include "EngineUtils.h"
#include "VCHeroCharacter.h"
#include "VCPlayerState.h"
#include "VCGameState.h"
#include "VCSpawner.h"
#include "VCTeam.h"
#include "VitaCoreLog.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "UnrealEd.h"
#endif


AVCGameMode::AVCGameMode()
{
}


void AVCGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AVCGameMode::StartPlay()
{
	if (UWorld* World = GetWorld())
	{
		VCGameState = World->GetGameState<AVCGameState>();
	}
	Super::StartPlay();	// Trigger BeginPlay for actors
}

void AVCGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AVCGameMode::Logout(AController * ExitingController)
{
	ExitingController->UnPossess();
	Super::Logout(ExitingController);
}

AVCCharacter* AVCGameMode::CreatePlayerCharacter(APlayerController* PlayerController, int CharacterIndex, ETeamType TeamType)
{
	UWorld * World = GetWorld();
	if(World == nullptr || PlayerController == nullptr || VCGameState == nullptr){
		return nullptr;
	}
	// Get character class
	const TSubclassOf<AVCCharacter>& CharacterClass = VCGameState->GetPlayableCharacters()[CharacterIndex];
	
	if ( CharacterClass == nullptr ){
		return nullptr;
	}
	
	AVCTeam* CharacterTeam = VCGameState->GetTeam(TeamType);
	const FTransform SpawnTransform = CharacterTeam ? CharacterTeam->GetSpawnTransform() : FTransform();
	
	AVCCharacter* NewCharacter = World->SpawnActorDeferred<AVCCharacter>(CharacterClass, FTransform(SpawnTransform));
		NewCharacter->SetTeamType(TeamType);
		if(CharacterTeam)
		{
			CharacterTeam->JoinTeam(NewCharacter);
		}
	NewCharacter->FinishSpawning(SpawnTransform);

	return NewCharacter;
}


#if WITH_EDITOR

void AVCGameMode::CreateTeamActors()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	TArray<AVCTeam*> Teams;
	for (TActorIterator<AVCTeam> TeamItr(World); TeamItr; ++TeamItr)
	{
		Teams.Add(*TeamItr);
	}

	for (auto& TeamActor : TeamClasses)
	{
		if (AVCTeam* TeamClass = Cast<AVCTeam>(TeamActor))
		{
			bool ShouldSpawn = true;
			for (auto& WorldTeam : Teams)
			{
				if (TeamClass->GetTeamType() == WorldTeam->GetTeamType())
				{
					ShouldSpawn = false;
					break;
				}
			}
			if (ShouldSpawn)
				World->SpawnActor<AVCTeam>(TeamActor, FTransform());
		}
	}
}

void AVCGameMode::RefreshTeamActors()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<AVCTeam> TeamItr(World); TeamItr; ++TeamItr)
	{
		TeamItr->Destroy();
	}
	for (auto& TeamActor : TeamClasses)
	{
		World->SpawnActor<AVCTeam>(TeamActor, FTransform());
	}
}
#endif
