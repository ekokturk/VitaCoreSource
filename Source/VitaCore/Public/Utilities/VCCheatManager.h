// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "VCCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(Exec) void VC_SetPlayerName(FString PlayerName);
	UFUNCTION(Exec) void VC_KillSelf();
	UFUNCTION(Exec) void VC_ResurrectSelf();
	UFUNCTION(Exec) void VC_LevelUp();


	// ================ CORE GAME ================
	
	UFUNCTION(Exec) void VC_NextMatchStage();

	//---- RPC Calls for cheat commands
protected:
	UFUNCTION(Server, Unreliable, WithValidation) void ServerNextMatchStage();
	UFUNCTION(Server, Unreliable, WithValidation) void ServerKillSelf();
	UFUNCTION(Server, Unreliable, WithValidation) void ServerResurrectSelf();
	UFUNCTION(Server, Unreliable, WithValidation) void ServerLevelUp();

};
