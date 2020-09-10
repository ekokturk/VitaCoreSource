// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "VCDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UVCDamageExecution();

	FProperty * HealthProperty;

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;



};
