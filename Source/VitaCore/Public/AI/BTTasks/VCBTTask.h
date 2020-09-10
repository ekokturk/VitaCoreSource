// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "VCBTTask.generated.h"

class AVCAIController;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class VITACORE_API UVCBTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UVCBTTask();

protected:
	virtual void SetOwner(AActor* ActorOwner) override;

	UPROPERTY(Transient)
		AVCAIController* VCAIController;
};
