// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCorePlayerController.h"

#include "AkGameplayStatics.h"

#include "VCCharacter.h"

AVCCorePlayerController::AVCCorePlayerController()
{
}

void AVCCorePlayerController::HandlePossess(AVCCharacter* InCharacter)
{
	Super::HandlePossess(InCharacter);
	
	SetCommanderAkSwitch(MyCharacter->GetTeamType());
	MyCharacter->OnTeamChanged.AddUObject(this, &AVCCorePlayerController::OnPawnTeamChanged);
}

void AVCCorePlayerController::HandleUnPossess(AVCCharacter* OutCharacter)
{
	Super::HandleUnPossess(OutCharacter);
	MyCharacter->OnTeamChanged.RemoveAll(this);
}

void AVCCorePlayerController::OnPawnTeamChanged(ETeamType OldTeam, ETeamType NewTeam)
{
	SetCommanderAkSwitch(NewTeam);
}

void AVCCorePlayerController::SetCommanderAkSwitch(ETeamType TeamType) const
{
	UEnum* TeamEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeamType"), true);
	const FString DisplayString = TeamEnum->GetDisplayNameTextByIndex(static_cast<int>(TeamType)).ToString();
	
	UAkGameplayStatics::SetSwitch(nullptr, MyCharacter ,TEXT("AkSwitch_Core_CommanderType"), FName(*DisplayString));
}

