// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VCGameTypes.h"
#include "VCGameMode.generated.h"

class AVCCharacter;
class AVCGameState;
class AVCTeam;

UCLASS()
class VITACORE_API AVCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVCGameMode();

public:
	// Create a player character and assign it to the team actor
	UFUNCTION(BlueprintCallable)
		virtual AVCCharacter* CreatePlayerCharacter(APlayerController * PlayerController, int CharacterIndex, ETeamType TeamType);
	
protected:
	// List of team actors that will be created for the gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		TArray<TSubclassOf<AVCTeam>> TeamClasses;

	// Cached game state
	UPROPERTY(Transient) AVCGameState* VCGameState;
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Logout(AController* ExitingController) override;

#if WITH_EDITOR
public:
	// Create team actors without changing already existing ones
	UFUNCTION() void CreateTeamActors();
	// Clear all teams from the world and recreate them
	UFUNCTION() void RefreshTeamActors();
#endif
};
