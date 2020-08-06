// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCShooterComponentTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"

FProjectileShootParams::FProjectileShootParams()
	:Count(1),
	DelayBetweenShots(0.0f),
	InitialSpeed(500.0f),
	ProjectileShootType(EProjectileShootTypes::DEFAULT)
{
}

FLaserShootParams::FLaserShootParams()
{
}


FShooterTimerProps::FShooterTimerProps()
	:PassedTimeCounter(0.001f),
	CurrentTimeTick(0)
{
}

void FShooterTimerProps::Clear(FTimerManager & TimerManager)
{
	TimerManager.ClearTimer(TimerHandle);
	PassedTimeCounter = 0.001f;
	CurrentTimeTick = 0;
}
