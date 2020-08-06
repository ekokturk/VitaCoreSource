// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "VCCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API AVCCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
		AActor * GetViewTargetActor() const;

	UFUNCTION(BlueprintCallable)
		AActor * GetPendingViewTargetActor() const;

};
