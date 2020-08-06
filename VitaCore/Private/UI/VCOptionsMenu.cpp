// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCOptionsMenu.h"

#include "UMG/Public/Components/Button.h"
#include "Engine/Engine.h"

bool UVCOptionsMenu::Initialize()
{
	bool bResult = Super::Initialize();

	if (!bResult)
	{
		return false;
	}

	return true;
}

void UVCOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

