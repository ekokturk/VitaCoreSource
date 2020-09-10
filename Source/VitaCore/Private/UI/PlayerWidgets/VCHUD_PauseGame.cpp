// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCHUD_PauseGame.h"
#include "VCHUD_Player.h"
#include "CanvasPanel.h"


bool UVCHUD_PauseGame::Initialize()
{
	bool bResult = Super::Initialize();

	// Check for base class
	if (!bResult)
	{
		return false;
	}

	InGameMenuCanvas = Cast<UCanvasPanel>(GetWidgetFromName("InGameMenuCanvas"));
	//UVCOptionsMenu = CreateWidget<UVCHUD_PauseGame>(this, UVCCan);
	
	return true;
}

void UVCHUD_PauseGame::SetMenuActive(bool IsActive)
{
	if(bIsTransitioning != true)
	{
		bIsCurrentlyActive = IsActive;
		BP_SetMenuActive(IsActive);
	}

}

bool UVCHUD_PauseGame::IsCurrentlyActive() const
{
	return bIsCurrentlyActive;
}






