// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCHUD_CoreGame.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "VCCoreGameState.h"
#include "VCPlayerController.h"
#include "VCWidget_Indicator.h"
#include "VCWidget_CoreTitan.h"
#include "VCTitanCharacter.h"
#include "VCWidget_GameStats.h"
#include "VCTeam.h"
#include "CoreGame/VCCoreGameTypes.h"


bool UVCHUD_CoreGame::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	return true;
}


void UVCHUD_CoreGame::NativeConstruct()
{
	Super::NativeConstruct();
	VCCoreGameState = GetWorld()->GetGameState<AVCCoreGameState>();
	checkf(VCCoreGameState, TEXT("Cannot create this widget with a different game state than Core game!"))

	// Update HUD based on the game stage and listen to changes
	HandleCoreGameStageChanged();
	VCCoreGameState->OnCoreGameStageChanged.AddWeakLambda(this, [=](EGameStages Prev, EGameStages Current)
	{
		HandleCoreGameStageChanged();
	});

	// Update HUD based on the team data and listen to changes
	HandleTeamDataChanged();
	VCCoreGameState->OnCoreTeamDataChanged.AddWeakLambda(this, [=](const FCoreTeamDataMap& DataMap)
	{
		HandleTeamDataChanged();
	});

	HandleFireTitan();
	HandleIceTitan();
	VCCoreGameState->OnTitanStateChanged.AddWeakLambda(this, [=](ETitanTypes TitanType, AVCTitanCharacter* TitanCharacter)
	{
		if(TitanType == ETitanTypes::Fire)
		{
			HandleFireTitan();
		}
		else if(TitanType == ETitanTypes::Ice)
		{
			HandleIceTitan();
		}
	});
	
}

void UVCHUD_CoreGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	HandleStartTimer();
	HandleVitaCoreTimer();
}

void UVCHUD_CoreGame::AddWidgetToCenterFeed(UUserWidget* Widget)
{
	Super::AddWidgetToCenterFeed(Widget);
	CenterFeed_VerticalBox->AddChildToVerticalBox(Widget);
}

void UVCHUD_CoreGame::AddWidgetToSideFeed(UUserWidget* Widget)
{
	Super::AddWidgetToSideFeed(Widget);
	RightFeed_VerticalBox->AddChildToVerticalBox(Widget);
}

void UVCHUD_CoreGame::TogglePlayerStats(bool IsActive)
{
	IsActive ?	PlayerStats_Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible) :
				PlayerStats_Widget->SetVisibility(ESlateVisibility::Hidden);
}


void UVCHUD_CoreGame::HandleTeamDataChanged() const
{
	FirstTeamCoreScore_Bar->SetPercent(VCCoreGameState->GetCoreDamageRatio(ETeamType::TEAM_VIOLET));
	FirstTeamScore_TextBlock->SetText(FText::FromString(FString::FromInt(VCCoreGameState->GetTeamMatchScore(ETeamType::TEAM_VIOLET))));
	SecondTeamCoreScore_Bar->SetPercent(VCCoreGameState->GetCoreDamageRatio(ETeamType::TEAM_GOLD));
	SecondTeamScore_TextBlock->SetText(FText::FromString(FString::FromInt(VCCoreGameState->GetTeamMatchScore(ETeamType::TEAM_GOLD))));
}

void UVCHUD_CoreGame::HandleFireTitan()
{
	if (VCCoreGameState->FireTitan)
	{
		FireTitan_Widget->SetVisibility(ESlateVisibility::Visible);
		FireTitan_IndicatorWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FireTitan_Widget->SetVisibility(ESlateVisibility::Hidden);
		FireTitan_IndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVCHUD_CoreGame::HandleIceTitan()
{
	if (VCCoreGameState->IceTitan)
	{
		IceTitan_Widget->SetVisibility(ESlateVisibility::Visible);
		IceTitan_IndicatorWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IceTitan_Widget->SetVisibility(ESlateVisibility::Hidden);
		IceTitan_IndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVCHUD_CoreGame::HandleVitaCoreTimer()
{
	if (VCCoreGameState->GetCurrentGameStage() == EGameStages::VITACORE)
	{
		VitaCoreTimer_TextBlock->SetText(FText());
		VitaCore_Image->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		VitaCoreTimer_TextBlock->SetText(FText::FromString(FString::FromInt(VCCoreGameState->GetVitaCoreActivationTimer())));
		VitaCore_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVCHUD_CoreGame::HandleStartTimer()
{
	const int StartTimer = VCCoreGameState->GetMatchStartTimer();
	StartTimer > 0 ?	StartTimer_TextBlock->SetText(FText::FromString(FString::FromInt(VCCoreGameState->GetMatchStartTimer()))):
						StartTimer_TextBlock->SetText(FText::GetEmpty());
}

void UVCHUD_CoreGame::HandleCoreGameStageChanged() 
{
	GameState_Switcher->SetActiveWidgetIndex(static_cast<int>(VCCoreGameState->GetCurrentGameStage()));
	if (VCCoreGameState->GetCurrentGameStage() == EGameStages::FINAL)
	{
		Gameplay_Canvas->SetVisibility(ESlateVisibility::Hidden);
		VCCoreGameState->GetMatchWinner() == MyCharacter->GetTeamType() ? BP_OnWin() : BP_OnLose();
	}
	else if (VCCoreGameState->GetCurrentGameStage() == EGameStages::INITIAL)
	{
		GeneralState_Canvas->SetVisibility(ESlateVisibility::Hidden);
		HandleStartTimer();
	}
	else
	{
		GeneralState_Canvas->SetVisibility(ESlateVisibility::Visible);
	}
}

