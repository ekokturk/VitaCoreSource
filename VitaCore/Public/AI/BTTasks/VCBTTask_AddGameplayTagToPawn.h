// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCBTTask.h"
#include "VCBTTask_AddGameplayTagToPawn.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "VC Add GameplayTag To Pawn"))
class VITACORE_API UVCBTTask_AddGameplayTagToPawn : public UVCBTTask
{
	GENERATED_BODY()
public:
	UVCBTTask_AddGameplayTagToPawn();

private:

	// Tag to be applied to the target (This will only be applied on Server)
	UPROPERTY(EditAnywhere)
		FGameplayTag Tag_ToBeAdded;

	// Tag will be applied after this delay
	UPROPERTY(EditAnywhere)
		FVector2D RandomDelayInterval;

	// If this is true, gameplay tag will act as execution and it will be removed
	UPROPERTY(EditAnywhere)
		bool bShouldRemoveTagAfterAdded;

	FTimerHandle TimerHandle_Delay;
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
