// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCTeam.h"


#include "AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "UnrealNetwork.h"
#include "VCCharacter.h"
#include "VCGameState.h"
#include "VCSpawner.h"
#include "VitaCoreLog.h"

AVCTeam::AVCTeam()
	:TeamType(ETeamType::NEUTRAL),
	TeamName("Team Neutral"),
	bIsPlayerTeam(false),
	bShouldRespawnMemberCharacters(true),
	TeamColor(FLinearColor::White),
	TeamLevel(0),
	TeamXP(0),
	CharacterRespawnTime(5.f)
{
	SetReplicates(true);
	bAlwaysRelevant = true;
}

void AVCTeam::PostActorCreated()
{
	Super::PostActorCreated();
#if WITH_EDITOR
	// Organize teams in editor
	SetActorLabel(TeamName.ToString());
	SetFolderPath("/Team");
#endif
}

void AVCTeam::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Cache team spawner references
	for(TActorIterator<AVCSpawner> SpawnerItr(GetWorld()); SpawnerItr; ++SpawnerItr)
	{
		ETeamType SpawnerTeamType;
		if(SpawnerItr->GetTeamType(SpawnerTeamType) && SpawnerTeamType==TeamType)
		{
			TeamSpawners.Add(*SpawnerItr);
		}
	}

	// Initialize level and XP from default values
	if(GetLocalRole() == ROLE_Authority)
	{
		InitLevelAndXP();
	}
}

void AVCTeam::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		VCGameState = World->GetGameState<AVCGameState>();
	}
}

void AVCTeam::SetDefaultTeamType(ETeamType NewTeamType)
{
	TeamType = NewTeamType;
}

void AVCTeam::SetDefaultTeamName(FName NewTeamName)
{
	TeamName = NewTeamName;
}

void AVCTeam::SetDefaultTeamColor(FLinearColor NewTeamColor)
{
	TeamColor = NewTeamColor;
}

void AVCTeam::InitLevelAndXP()
{
	if (VCGameState)
	{
		TeamLevel = FMath::Clamp(TeamLevel, 0, VCGameState->GetMaxLevel());
		TeamXP = FMath::Clamp(TeamXP, 0, TeamXP);
		const int CurrentLevelMinXP = VCGameState->GetLevelBracketTotalXP(TeamLevel - 1);
		if (TeamXP < CurrentLevelMinXP)
		{
			TeamXP = CurrentLevelMinXP;
		}
	}
}

void AVCTeam::ApplyXP(int XP)
{
	if(VCGameState)
	{
		TeamXP = FMath::Clamp(TeamXP + XP,0, TeamXP + XP);
		int CurrentLevelBracket = VCGameState->GetLevelBracketTotalXP(TeamLevel);
		int PreviousLevelBracket = VCGameState->GetLevelBracketXP(TeamLevel - 1);
		// Skip to higher level based on xp
		while (CurrentLevelBracket <= TeamXP && TeamLevel < VCGameState->GetMaxLevel())
		{
			TeamLevel++;
			CurrentLevelBracket = VCGameState->GetLevelBracketTotalXP(TeamLevel);
		}
		// Fall to lower level based on xp
		while(PreviousLevelBracket > TeamXP && TeamLevel > 0)
		{
			TeamLevel--;
			PreviousLevelBracket = VCGameState->GetLevelBracketXP(TeamLevel - 1);
		}

		// Send
		CurrentLevelBracket = VCGameState->GetLevelBracketTotalXP(TeamLevel);
		PreviousLevelBracket = VCGameState->GetLevelBracketXP(TeamLevel - 1);
		OnXPChanged.Broadcast(XP, TeamXP, TeamXP - PreviousLevelBracket, CurrentLevelBracket - PreviousLevelBracket);
	}
	else
	{
		VC_LOG(Error, "Game State cache is missing for Team Actor! Error in applying experience.")
	}
}

void AVCTeam::OnRep_TeamXP(int PrevXP)
{
	const int CurrentLevelBracketXP = VCGameState ? VCGameState->GetLevelBracketTotalXP(TeamLevel) : 0.f;
	const int PreviousLevelBracketXP = VCGameState ? VCGameState->GetLevelBracketTotalXP(TeamLevel - 1) : 0.f;
	OnXPChanged.Broadcast(TeamXP - PrevXP, TeamXP, TeamXP - PreviousLevelBracketXP, 
		CurrentLevelBracketXP - PreviousLevelBracketXP);
}

EAffiliationType AVCTeam::GetTeamAffiliation(ETeamType OtherTeamType) const
{
	if(OtherTeamType == ETeamType::NEUTRAL)
	{
		return EAffiliationType::NEUTRAL;
	}
	return OtherTeamType == TeamType ? EAffiliationType::ALLY : EAffiliationType::ENEMY;
}

void AVCTeam::IncrementLevel()
{
	// Use experience to level up
	if(VCGameState)
	{
		const int RequiredXP = VCGameState->GetLevelBracketTotalXP(TeamLevel) - GetTeamXP();
		ApplyXP(RequiredXP);
		return;
	}
	else
	{
		VC_LOG(Error, "Game State cache is missing for Team Actor! Error in applying experience on increment level.")
	}
	// Increment Team level directly
	TeamLevel++;
}



bool AVCTeam::JoinTeam(AVCCharacter* JoiningCharacter)
{
	OnCharacterJoinedTeam.Broadcast(JoiningCharacter);
	
	if(TeamMemberCharacters.Find(JoiningCharacter) == INDEX_NONE)
	{
		TeamMemberCharacters.Add(JoiningCharacter);
		for(auto & InfiniteEffect : InfiniteTeamEffects)
		{
			if(JoiningCharacter->GetAbilitySystemComponent())
			{
				JoiningCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*InfiniteEffect.Data.Get());
			}
		}
		return true;
	}
	return false;
}

bool AVCTeam::LeaveTeam(AVCCharacter* LeavingCharacter)
{
	OnCharacterLeftTeam.Broadcast(LeavingCharacter);
	
	const int Index = TeamMemberCharacters.Find(LeavingCharacter);
	if (Index != INDEX_NONE)
	{
		TeamMemberCharacters.RemoveAt(Index);
		for (auto& InfiniteEffect : InfiniteTeamEffects)
		{
			if (LeavingCharacter->GetAbilitySystemComponent() && InfiniteEffect.Data.IsValid())
			{
				if(const UGameplayEffect * Effect = InfiniteEffect.Data->Def)
				{
					LeavingCharacter->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(Effect->GetClass(), 
						InfiniteEffect.Data->GetEffectContext().GetInstigatorAbilitySystemComponent());
				}
			}
		}
		return true;
	}
	return false;
}

AVCSpawner* AVCTeam::GetSpawner(int SpawnerIndex, bool ShouldGetOnlyActive) const
{
	if(TeamSpawners.Num() != 0)
	{
		for(int i=0; i < TeamSpawners.Num(); i++)
		{
			if(TeamSpawners[i]->IsActive() == false && ShouldGetOnlyActive)
			{
				continue;
			}

			if(SpawnerIndex <= i)
			{
				return TeamSpawners[i];
			}
		}
	}
	return nullptr;
}

FTransform AVCTeam::GetSpawnTransform(int SpawnerIndex) const
{
	const AVCSpawner * Spawner = GetSpawner(SpawnerIndex, true);
	if(Spawner)
	{
		return Spawner->GetSpawnTransform();
	}
	
	return GetActorTransform();
}

bool AVCTeam::SpawnCharacter(AVCCharacter * Character) const
{
	check(HasAuthority())
	
	if(bShouldRespawnMemberCharacters)
	{
		const int CharacterIndex = TeamMemberCharacters.IndexOfByKey(Character);
		Character->SetActorTransform(GetSpawnTransform(CharacterIndex));
		return true;
	}
	return false;
}

void AVCTeam::ApplyTeamEffect(FGameplayEffectSpecHandle EffectSpecHandle)
{
	for(auto Character : TeamMemberCharacters)
	{
		if(Character && Character->GetAbilitySystemComponent())
		{
			Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			if(EffectSpecHandle.Data.IsValid())
			{
				const UGameplayEffect* Effect = EffectSpecHandle.Data->Def;
				if(Effect && Effect->DurationPolicy == EGameplayEffectDurationType::Infinite)
				{
					InfiniteTeamEffects.Add(EffectSpecHandle);
				}
			}
		}
	}
	
}

void AVCTeam::RemoveTeamEffect(FGameplayEffectSpecHandle EffectSpecHandle)
{
	for (auto Character : TeamMemberCharacters)
	{
		if (Character && Character->GetAbilitySystemComponent())
		{
			if (Character && Character->GetAbilitySystemComponent() && EffectSpecHandle.Data.Get())
			{
				if(const UGameplayEffect * Effect = EffectSpecHandle.Data->Def)
				{
					UAbilitySystemComponent * InstigatorASC = EffectSpecHandle.Data.Get()->
						GetEffectContext().GetInstigatorAbilitySystemComponent();
					Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(Effect->GetClass(), InstigatorASC);

					for (int i = 0; i < InfiniteTeamEffects.Num(); i++)
					{
						if (InfiniteTeamEffects[i].Data->Def == Effect)
						{
							InfiniteTeamEffects.RemoveAt(i);
							i--;
						}
					}
				}
			}
			
		}
	}
}

void AVCTeam::OnRep_TeamLevel(int PrevLevel)
{
	OnTeamLevelChanged.Broadcast(PrevLevel, TeamLevel);
}

void AVCTeam::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVCTeam, TeamLevel);
	DOREPLIFETIME(AVCTeam, TeamXP);
}

