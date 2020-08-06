// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCBTDecorator.h"
#include "VCBTDecorator_InRange.generated.h"

/**
 * 
 */

// TODO: Make Line of sight a option

UCLASS(meta = (DisplayName = "VC In Range"))
class VITACORE_API UVCBTDecorator_InRange : public UVCBTDecorator
{
	GENERATED_BODY()

public:
	UVCBTDecorator_InRange();

	UPROPERTY(EditAnywhere)
		float RangeToCheck;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
#if WITH_EDITOR
	virtual void BuildDescription() override;
#endif
	
};
