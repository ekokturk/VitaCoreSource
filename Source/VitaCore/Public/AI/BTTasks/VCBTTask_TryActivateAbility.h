// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCBTTask.h"
#include "VCBTTask_TryActivateAbility.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "VC Try Activate Ability"))
class VITACORE_API UVCBTTask_TryActivateAbility : public UVCBTTask
{
	GENERATED_BODY()

public:
	UVCBTTask_TryActivateAbility();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// TODO - Expose this to Editor with a dynamic enum (?)
	// Id of the ability to be activated
	UPROPERTY(EditAnywhere, Category = "BTTask - TryActivateAbility")
		int32 AbilityID;
};
