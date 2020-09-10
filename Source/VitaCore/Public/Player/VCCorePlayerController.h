// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/VCPlayerController.h"
#include "VCGameTypes.h"
#include "VCCorePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API AVCCorePlayerController : public AVCPlayerController
{
	GENERATED_BODY()

public:
	AVCCorePlayerController();

protected:
	virtual void HandlePossess(AVCCharacter* InCharacter) override;
	virtual void HandleUnPossess(AVCCharacter* OutCharacter) override;
	
	UFUNCTION() void OnPawnTeamChanged(ETeamType OldTeam, ETeamType NewTeam);

// =============== AUDIO ====================	
protected:
	void SetCommanderAkSwitch(ETeamType TeamType) const;


	
};
