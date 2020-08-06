// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCCoreGameMode.h"
#include "VCCoreGameState.h"
#include "VCVitaCoreShrine.h"
#include "VCCoreGameTypes.h"
#include "EngineUtils.h"
#include "VCSpawner.h"
#include "VCPropController.h"
#include "TimerManager.h"
#include "VCHeroCharacter.h"
#include "VitaCoreLog.h"

AVCCoreGameMode::AVCCoreGameMode()
	:ScoreToWin(2),
	VitaCoreActivationTimer(60),
	bIsFireTitanSpawned(false),
	bIsIceTitanSpawned(false),
	CoreCaptureDamage(10000.0f),
	MatchStartWaitTime(15),
	MatchStartFinalCountdown(5)
{
}

void AVCCoreGameMode::BeginPlay()
{
	Super::BeginPlay();

	CoreGameState = GetGameState<AVCCoreGameState>();

	InitializeActors();

	checkf(CoreGameState, TEXT("VCCoreGameState is not found!"))
	CoreGameState->VitaCoreActivationCounter = VitaCoreActivationTimer;
	CoreGameState->MatchStartCounter = MatchStartWaitTime;

}

void AVCCoreGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CountVitaCoreTimer(DeltaTime);		// Changes state of the game depending on VitaCore counter
}

void AVCCoreGameMode::GoNextGameStage()
{
	switch (CoreGameState->CurrentGameStage)
	{
	case EGameStages::INITIAL:
		SetGameCurrentStage(EGameStages::SCOUTING);
		
		for (AVCCharacter* PlayerCharacter : CoreGameState->GetAllPlayerCharacters())
		{
			PlayerCharacter->RefreshAbilities();
			PlayerCharacter->DoSpawn();
		}
		CoreGameState->bIsMatchStarted = true;
		GetWorldTimerManager().ClearTimer(TimerHandle_MatchStartCounter);
		break;

	case EGameStages::SCOUTING:
		ResetCore();
		SetGameCurrentStage(EGameStages::VITACORE);
		break;

	case EGameStages::VITACORE:

		if (IsMatchWon() == true)
		{
			SetGameCurrentStage(EGameStages::FINAL);
			if (VitaCoreShrine)
			{
				VitaCoreShrine->DoCapture(CoreGameState->GetTeam(CoreGameState->MatchWinner));
			}
		}
		else
		{
			if (VitaCoreShrine)
			{
				VitaCoreShrine->SetShieldActive(true);
			}
			if (bIsFireTitanSpawned == false)
			{
				SetGameCurrentStage(EGameStages::FIRETITAN);
			}
			else
			{
				SetGameCurrentStage(EGameStages::ICETITAN);
			}
			SpawnTitan();
		}

		break;

	case EGameStages::FIRETITAN:
		ResetCore();
		SetGameCurrentStage(EGameStages::VITACORE);
		break;

	case EGameStages::ICETITAN:
		ResetCore();
		SetGameCurrentStage(EGameStages::VITACORE);
		break;

	case EGameStages::FINAL:
		break;
	}
}

void AVCCoreGameMode::SetGameCurrentStage(EGameStages NewGameStage) const
{
	const EGameStages PrevGameStage = CoreGameState->GetCurrentGameStage();
	CoreGameState->CurrentGameStage = NewGameStage;
	CoreGameState->OnRep_CurrentGameStage(PrevGameStage);
}

bool AVCCoreGameMode::IsMatchWon() const
{
	for(const auto& TeamDataPair : CoreGameState->TeamDataMap.Map)
	{
		if(TeamDataPair.Value.GetMatchScore() >= ScoreToWin)
		{
			CoreGameState->MatchWinner = TeamDataPair.Key;
			return true;
		}
	}

	return false;
}

void AVCCoreGameMode::StartVitaCoreMatch()
{
	if (TimerHandle_MatchStartCounter.IsValid() == false)
	{
		if(CoreGameState->bIsMatchStarted == false)
		{
			MatchStartWaitTime <= 0 ?	GoNextGameStage() :
										GetWorld()->GetTimerManager().SetTimer(TimerHandle_MatchStartCounter, this,
											&AVCCoreGameMode::MatchStartCounterTimer, 1.0f, true);
		}
	}
}

void AVCCoreGameMode::AddCoreDamage(ETeamType DamagerTeamType, float Damage)
{
	// Add Damage to team
	if(CoreGameState->TeamDataMap.HasTeam(DamagerTeamType))
	{
		CoreGameState->TeamDataMap[DamagerTeamType].AddCoreDamage(Damage);
		CoreGameState->OnCoreTeamDataChanged.Broadcast(CoreGameState->TeamDataMap);
	}

	// Check if vitacore shrine is captured
	if(VerifyCoreCapture(DamagerTeamType))
	{
		if(VitaCoreShrine)
		{
			VitaCoreShrine->SetShieldActive(true);
		}
		GoNextGameStage();
	}

}

void AVCCoreGameMode::CountVitaCoreTimer(float DeltaSeconds)
{
	if (CoreGameState->bIsMatchStarted == true)
	{
		if (CoreGameState->CurrentGameStage == EGameStages::FIRETITAN ||
			CoreGameState->CurrentGameStage == EGameStages::ICETITAN ||
			CoreGameState->CurrentGameStage == EGameStages::SCOUTING)
		{
			CoreGameState->VitaCoreActivationCounter -= DeltaSeconds;
			CoreGameState->VitaCoreActivationCounter = FMath::Clamp(CoreGameState->VitaCoreActivationCounter, 0.0f, CoreGameState->VitaCoreActivationCounter);
			if (CoreGameState->VitaCoreActivationCounter <= 0)
			{
				GoNextGameStage();
			}
		}
	}
}

void AVCCoreGameMode::ResetCore() const
{

	const int TotalScore = CoreGameState->GetTeamMatchScore(ETeamType::TEAM_GOLD) + CoreGameState->GetTeamMatchScore(ETeamType::TEAM_VIOLET);
	CoreGameState->CoreCaptureDamage = CoreCaptureDamage * (1 + CoreCaptureMultiplier * TotalScore);

	for(auto & TeamDataPair : CoreGameState->TeamDataMap.Map)
	{
		TeamDataPair.Value.ResetCoreDamage();
		CoreGameState->OnCoreTeamDataChanged.Broadcast(CoreGameState->TeamDataMap);
	}
	
	CoreGameState->VitaCoreActivationCounter = VitaCoreActivationTimer;
	if (VitaCoreShrine)
	{
		VitaCoreShrine->SetShieldActive(false);
		const float HitPoints = CoreGameState->GetAllTeams().Num() * CoreCaptureDamage;
		VitaCoreShrine->SetHitPoints(HitPoints);
	}
}



bool AVCCoreGameMode::VerifyCoreCapture(ETeamType PlayerTeam) const
{
	if (CoreGameState->GetCoreDamageRatio(PlayerTeam) == 1.0f &&
		CoreGameState->TeamDataMap.HasTeam(PlayerTeam))
	{
		CoreGameState->TeamDataMap[PlayerTeam].AddMatchScore();
		CoreGameState->OnCoreTeamDataChanged.Broadcast(CoreGameState->TeamDataMap);
		CoreGameState->RoundWinner = PlayerTeam;
		CoreGameState->BP_OnRep_RoundWinner();
		CoreGameState->SendMessage_VitaCoreCapture(PlayerTeam);
		return true;
	}
	return false;
}

void AVCCoreGameMode::SpawnTitan()
{
	UWorld * World = GetWorld();
	if (World != nullptr)
	{
		// Spawn fire titan if it is not spawned
		if (bIsFireTitanSpawned == false)
		{
			if (FireTitanSpawner != nullptr)
			{
				FireTitanSpawner->DoSpawn();
				CoreGameState->OnRep_FireTitan();
				if(PropController != nullptr)
				{
					PropController->SetFireTitanPropsActive(true);
				}
			}
			bIsFireTitanSpawned = true;
		}
		// Spawn ice titan if it is not spawned
		else if(bIsIceTitanSpawned == false)
		{
			if(IceTitanSpawner != nullptr)
			{
				IceTitanSpawner->DoSpawn();
				CoreGameState->OnRep_IceTitan();
				if (PropController != nullptr)
				{
					PropController->SetIceTitanPropsActive(true);
				}
			}
			bIsIceTitanSpawned = true;
		}
	}
}

void AVCCoreGameMode::HandleCoreDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth)
{
	AVCCharacter* DamagerCharacter = Cast<AVCCharacter>(Damager);
	if(DamagerCharacter && CoreGameState->TeamDataMap.HasTeam(DamagerCharacter->GetTeamType()))
	{
		AddCoreDamage(DamagerCharacter->GetTeamType(), Damage);
	}
}


void AVCCoreGameMode::InitializeActors()
{
	UWorld * World = GetWorld();
	if (World != nullptr)
	{
		// VitaCore Character
		for (TActorIterator<AVCVitaCoreShrine> ActorItr(World); ActorItr; ++ActorItr)
		{
			VitaCoreShrine = *ActorItr;
		}
		//checkf(PropController, TEXT("VitaCoreShrine is missing in the map!"))

		if(VitaCoreShrine)
		{
			VitaCoreShrine->GetOnDamageDelegate().AddUObject(this, &AVCCoreGameMode::HandleCoreDamage); 
		}

		for (TActorIterator<AVCPropController> ActorItr(World); ActorItr; ++ActorItr)
		{
			PropController = *ActorItr;
		}
		//checkf(PropController, TEXT("PropController is missing in the map!"))

		// Initialize Spawners for the titans
		for (TActorIterator<AVCSpawner> ActorItr(World); ActorItr; ++ActorItr)
		{
			if(ActorItr)
			{
				if(ActorItr->ActorHasTag("FireTitan"))
				{
					FireTitanSpawner = *ActorItr;
				}
				else if (ActorItr->ActorHasTag("IceTitan"))
				{
					IceTitanSpawner = *ActorItr;
				}
			}
		}
		
		//checkf(FireTitanSpawner, TEXT("Spawner for Fire Titan is missing in the map!"))
		//checkf(IceTitanSpawner, TEXT("Spawner for Ice Titan is missing in the map!"))
	}
}

void AVCCoreGameMode::MatchStartCounterTimer()
{
	CoreGameState->MatchStartCounter = FMath::Clamp(CoreGameState->MatchStartCounter - 1, 0, MatchStartWaitTime);
	if(CoreGameState->MatchStartCounter == 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_MatchStartCounter);
		GoNextGameStage();
	}
}




