// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTTask.h"

#include "VCAIController.h"

UVCBTTask::UVCBTTask()
{
	// Instance task
	bCreateNodeInstance = true;
}

void UVCBTTask::SetOwner(AActor* ActorOwner)
{
	// Cache VC AI controller
	VCAIController = Cast<AVCAIController>(ActorOwner);
}
