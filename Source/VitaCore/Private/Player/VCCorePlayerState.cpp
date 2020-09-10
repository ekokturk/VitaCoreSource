// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCorePlayerState.h"
#include "Net/UnrealNetwork.h"

AVCCorePlayerState::AVCCorePlayerState()
{

}

// Replicate properties
void AVCCorePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AVCCorePlayerState, Stats);

}