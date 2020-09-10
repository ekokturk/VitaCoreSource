// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_GameStats.h"

bool UVCWidget_GameStats::Initialize()
{
	bool bResult = Super::Initialize();

	if (!bResult)
	{
		return false;
	}

	return true;
}


void UVCWidget_GameStats::NativeConstruct()
{
	Super::NativeConstruct();
	//UWorld * World = GetWorld();
	//if (World != nullptr)
	//{
	//	MyGameState = World->GetGameState<AVCCoreGameState>();
	//}
}

void UVCWidget_GameStats::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MyGameState == nullptr)
	{
		return;
	}

}
