// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCheatManager.h"

#include "VCCharacter.h"
#include "VCCoreGameMode.h"
#include "VCPlayerController.h"



void UVCCheatManager::VC_KillSelf()
{
	ServerKillSelf();
}

void UVCCheatManager::ServerKillSelf_Implementation()
{
	//DoDeath();
}

bool UVCCheatManager::ServerKillSelf_Validate()
{
	return true;
}

void UVCCheatManager::VC_ResurrectSelf()
{
	ServerResurrectSelf();
}

void UVCCheatManager::ServerResurrectSelf_Implementation()
{
	//DoSpawn();
}

bool UVCCheatManager::ServerResurrectSelf_Validate()
{
	return true;
}

void UVCCheatManager::VC_LevelUp()
{
	ServerLevelUp();
}

void UVCCheatManager::ServerLevelUp_Implementation()
{
	AVCCharacter* MyCharacter = Cast<AVCCharacter>(GetOuterAPlayerController()->GetPawn());
	if (MyCharacter)
	{
		MyCharacter->IncrementLevel();
	}
}

bool UVCCheatManager::ServerLevelUp_Validate() { return true; }


void UVCCheatManager::VC_SetPlayerName(FString PlayerName)
{
	if(auto PlayerController = Cast<AVCPlayerController>(GetOuterAPlayerController()))
	{
		PlayerController->ServerSetPlayerName(PlayerName);
	}
}



void UVCCheatManager::VC_NextMatchStage()
{
	ServerNextMatchStage();
}

void UVCCheatManager::ServerNextMatchStage_Implementation()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		AVCCoreGameMode* MyGameMode = World->GetAuthGameMode<AVCCoreGameMode>();
		if (MyGameMode != nullptr)
		{
			MyGameMode->GoNextGameStage();
		}
	}
}

bool UVCCheatManager::ServerNextMatchStage_Validate()
{
	return true;
}


