// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/VCAIController.h"
#include "VCAIController_CoreTitan.generated.h"

class AVCCoreGameState;

/**
 * 
 */
UCLASS()
class VITACORE_API AVCAIController_CoreTitan : public AVCAIController
{
	GENERATED_BODY()

public:
	AVCAIController_CoreTitan();

protected:

	// Override Titan damage based on the damager distance
	UFUNCTION() void ProcessDamage(AActor* Damager, float& Damage) const;

	// Callback to notify when titan pawn is damaged
	UFUNCTION() void OnDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth);

	// Distance multiplier which will override the damage
	UFUNCTION() float GetDistanceMultiplier(AVCCharacter* OtherActor) const;

	
protected:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
};
