// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCHUD_TeamSelection.h"


#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "UniformGridPanel.h"
#include "VCGameState.h"
#include "VCPlayerController.h"
#include "VCPlayerState.h"
#include "VCTeam.h"
#include "VCWidget_ToggleButton.h"
#include "VerticalBox.h"
#include "WidgetTree.h"

bool UVCHUD_TeamSelection::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	ActiveIndex = -1;
	return true;
}

void UVCHUD_TeamSelection::NativeConstruct()
{
	Super::NativeConstruct();

	if(VCGameState)
	{
		int PlayableTeamIndex = 0;
		for(const auto Team : VCGameState->GetAllTeams())
		{
			if(Team->IsPlayerTeam())
			{
				UVCWidget_ToggleButton* TeamToggle = WidgetTree->ConstructWidget<UVCWidget_ToggleButton>(TeamToggleButtonClass);
				TeamToggle->SetBackgroundColor(Team->GetTeamColor());
				TeamToggle->SetImage(Team->GetTeamLogoImage());
				TeamToggle->OnToggleActiveStateChanged.AddWeakLambda(this, [=](bool IsActive) {
					if (IsActive || ActiveIndex == PlayableTeamIndex)
					{
						SelectTeamToggle(PlayableTeamIndex);
						//VCPlayerController->PostLocalAkEvent(AkEvent_ToggleSelect);
					}
				});
				TeamToggleButtons.Add(TPair<AVCTeam*, UVCWidget_ToggleButton*>(Team, TeamToggle));
				ToggleButtonContainer_GridPanel->AddChildToUniformGrid(TeamToggle, 0, PlayableTeamIndex);
				PlayableTeamIndex++;
			}
		}
	}

	Select_Button->OnClicked.AddDynamic(this, &UVCHUD_TeamSelection::HandleSelectButtonClicked);

	// Initialize HUD
	if(TeamToggleButtons.Num() > 0)
	{
		SelectTeamToggle(0);
	}
}

void UVCHUD_TeamSelection::SelectTeamToggle(int ToggleIndex)
{
	if (TeamToggleButtons.IsValidIndex(ToggleIndex) == false) {
		return;
	}

	for (auto& Toggle : TeamToggleButtons)
	{
		Toggle.Value->SetActive(false);
	}

	TeamToggleButtons[ToggleIndex].Value->SetActive(true);
	if (ActiveIndex != ToggleIndex)
	{
		ActiveIndex = ToggleIndex;
		HandleTeamSelect();
		UpdateCurrentPlayers();
	}
}

void UVCHUD_TeamSelection::HandleTeamSelect()
{
	if(const AVCTeam* Team = TeamToggleButtons[ActiveIndex].Key)
	{
		const FLinearColor TeamColor = Team->GetTeamColor();
		Background_Image->SetColorAndOpacity(TeamColor * FLinearColor(1.f,1.f,1.f,0.2));
		Select_Button->SetBackgroundColor(TeamColor);
	}
}

void UVCHUD_TeamSelection::HandleSelectButtonClicked()
{
	check(VCPlayerController->GetVCPlayerState())
	VCPlayerController->GetVCPlayerState()->OnTeamChanged.AddWeakLambda(this, [=](ETeamType PrevTeam, ETeamType CurrentTeam)
	{
		if(CurrentTeam == TeamToggleButtons[ActiveIndex].Key->GetTeamType())
		{
			SetWidgetActive(false);
		}
	});

	VCPlayerController->ServerSetTeam(TeamToggleButtons[ActiveIndex].Key->GetTeamType());

}

void UVCHUD_TeamSelection::UpdateCurrentPlayers()
{
	check(VCGameState)
	if(ActiveIndex < 0){
		return;
	}
	TeamPlayers_VerticalBox->ClearChildren();
	NoTeamPlayers_VerticalBox->ClearChildren();

	const TArray<AVCPlayerState*>& Players = VCGameState->GetPlayers();
	for(const auto& Player : Players)
	{
		if(Player->GetTeamType() == TeamToggleButtons[ActiveIndex].Key->GetTeamType())
		{
			UTextBlock* PlayerNameTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			PlayerNameTextBlock->Text = FText::FromString(Player->GetPlayerName());
			PlayerNameTextBlock->ColorAndOpacity = TeamToggleButtons[ActiveIndex].Key->GetTeamColor();
			TeamPlayers_VerticalBox->AddChildToVerticalBox(PlayerNameTextBlock);
		}
		else if(Player->GetTeamType() == ETeamType::NEUTRAL)
		{
			UTextBlock* PlayerNameTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			PlayerNameTextBlock->Text = FText::FromString(Player->GetPlayerName());
			NoTeamPlayers_VerticalBox->AddChildToVerticalBox(PlayerNameTextBlock);
		}
	}
}
