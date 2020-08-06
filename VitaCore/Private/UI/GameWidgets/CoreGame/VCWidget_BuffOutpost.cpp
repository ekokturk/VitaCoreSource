// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_BuffOutpost.h"
#include "VCBuffOutpost.h"
#include "ProgressBar.h"
#include "VCCoreGameState.h"
#include "VCTeam.h"

bool UVCWidget_BuffOutpost::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	CaptureAmountBar = Cast<UProgressBar>(GetWidgetFromName("CaptureAmountBar"));
	DefaultCaptureAmountColor = FColor::White;
	return true;
}

void UVCWidget_BuffOutpost::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVCWidget_BuffOutpost::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(MyOutpost == nullptr)
	{
		return;
	}

	// TODO Remove from tick to callbacks
	
	CaptureAmountBar->SetPercent(MyOutpost->GetCaptureAmount());

	const FLinearColor TeamColor = VCGameState->GetTeam(MyOutpost->GetCapturedTeam()) ?
		VCGameState->GetTeam(MyOutpost->GetCapturedTeam())->GetTeamColor() :
		DefaultCaptureAmountColor;
	
	CaptureAmountBar->SetFillColorAndOpacity(TeamColor);
	
	MyOutpost->IsLocked() ? CaptureAmountBar->SetIsEnabled(false) : CaptureAmountBar->SetIsEnabled(true);
}

void UVCWidget_BuffOutpost::SetOwningOutpost(AVCBuffOutpost* Outpost)
{
	MyOutpost = Outpost;
}
