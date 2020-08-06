// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCHUD_Game.h"

bool UVCHUD_Game::Initialize()
{
	bool bResult = Super::Initialize();

	if (!bResult)
	{
		return false;
	}

	ObjectiveDisplayDuration = 5.0f;

	return true;
}


void UVCHUD_Game::NativeConstruct()
{
	Super::NativeConstruct();
}


void UVCHUD_Game::ShowCurrentObjective()
{
	BP_ShowCurrentObjective();
}

void UVCHUD_Game::AddWidgetToSideFeed(UUserWidget* Widget)
{
	if (Widget == nullptr)
	{
		return;
	}
}

void UVCHUD_Game::AddWidgetToCenterFeed(UUserWidget* Widget)
{
	if(Widget == nullptr)
	{
		return;
	}
}

void UVCHUD_Game::TogglePlayerStats(bool IsActive)
{

}
