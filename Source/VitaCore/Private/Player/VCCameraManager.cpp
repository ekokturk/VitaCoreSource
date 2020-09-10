// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCCameraManager.h"

AActor* AVCCameraManager::GetViewTargetActor() const
{
	return GetViewTarget();
}

AActor* AVCCameraManager::GetPendingViewTargetActor() const
{
	return PendingViewTarget.Target;
}
