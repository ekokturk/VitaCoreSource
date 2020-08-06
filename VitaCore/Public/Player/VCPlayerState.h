// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VCGameTypes.h"
#include "VCEntityInterface.h"
#include "VCPlayerState.generated.h"

class AVCHeroCharacter;

UCLASS()
class VITACORE_API AVCPlayerState : public APlayerState
{
	GENERATED_BODY()

// ================== PLAYER NAME
public:

	AVCPlayerState();

	// Set player team type
	UFUNCTION() void SetTeamType(ETeamType NewTeamType);

	// Returns team type of the player
	UFUNCTION() ETeamType GetTeamType() const { return TeamType; }

	// Callback delegate to notify tema change
	FOnTeamChanged OnTeamChanged;

protected:

	// Notify change of team type on client
	UFUNCTION() virtual void OnRep_TeamType(ETeamType PrevTeam);
	
protected:

	// Team type of the player that will effect controlled pawn
	UPROPERTY(ReplicatedUsing=OnRep_TeamType, EditAnywhere, Category = "Config")
		ETeamType TeamType;

	// Default player name that will be set for the player on login
	UPROPERTY(EditAnywhere, Category = "Config") FString DefaultPlayerName;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



};
