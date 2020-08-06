// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VCPlayerState.h"
#include "VCCorePlayerState.generated.h"

USTRUCT(BlueprintType)
struct FCorePlayerStatistics
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) float XPContribution;
	UPROPERTY(BlueprintReadWrite) float BossDamage;
	UPROPERTY(BlueprintReadWrite) float VitaCoreDamage;
	UPROPERTY(BlueprintReadWrite) int MinionKillCount;
	UPROPERTY(BlueprintReadWrite) int PlayerKillCount;
	UPROPERTY(BlueprintReadWrite) int DeathCount;
	UPROPERTY(BlueprintReadWrite) int CrystalsCaptured;

	FCorePlayerStatistics()
	{

		XPContribution = 0.0f;
		CrystalsCaptured = 0;
		MinionKillCount = 0;
		PlayerKillCount = 0;
		DeathCount = 0;
		BossDamage = 0.0f;
		VitaCoreDamage = 0.0f;
	}

};

UCLASS()
class VITACORE_API AVCCorePlayerState : public AVCPlayerState
{
	GENERATED_BODY()

public:
	AVCCorePlayerState();

	UPROPERTY(Replicated, BlueprintReadWrite)
		FCorePlayerStatistics Stats;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
