// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCHUD_CharacterSelection.h"


#include "UniformGridPanel.h"
#include "VCCharacter.h"
#include "VCPlayerController.h"
#include "VCGameState.h"
#include "VCPlayerState.h"
#include "VCTeam.h"
#include "VerticalBox.h"
#include "WidgetTree.h"
#include "UMG/Public/Components/Button.h"
#include "VCWidget_ToggleButton.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

bool UVCHUD_CharacterSelection::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	ActiveIndex = -1;
	return true;
}

void UVCHUD_CharacterSelection::NativeConstruct()
{
	Super::NativeConstruct();

	check(VCPlayerController->GetVCPlayerState())
	
	if(VCGameState && VCPlayerController->GetVCPlayerState() &&  CharacterToggleButtonTemplate)
	{
		FLinearColor TeamColor = FLinearColor::White;
		AVCTeam* PlayerTeam = VCGameState->GetTeam(VCPlayerController->GetVCPlayerState()->GetTeamType());
		if(PlayerTeam)
		{
			TeamColor = PlayerTeam->GetTeamColor();
			DisplayedCharacterSpawnTransform = PlayerTeam->GetSpawnTransform();
			
		}
		
		const auto& PlayableCharacters = VCGameState->GetPlayableCharacters();
		for(int i=0; i < PlayableCharacters.Num(); i++)
		{
			AVCCharacter* Character = PlayableCharacters[i].GetDefaultObject();
			UVCWidget_ToggleButton* CharacterToggle = WidgetTree->ConstructWidget<UVCWidget_ToggleButton>(CharacterToggleButtonTemplate);
			CharacterToggle->SetImage(Character->GetAvatarImage());
			CharacterToggle->SetBackgroundColor(TeamColor);
			CharacterToggle->OnToggleActiveStateChanged.AddWeakLambda(this, [=](bool IsActive){
				if (IsActive || ActiveIndex == i)
				{
					SelectCharacterToggle(i);
					VCPlayerController->PostLocalAkEvent(AkEvent_ToggleSelect);
				}
			});
			CharacterToggleButtons.Add(CharacterToggle);
			ToggleButtonContainer_GridPanel->AddChildToUniformGrid(CharacterToggle, i);
		}
		
	}
	if(CharacterToggleButtons.Num() > 0)
	{
		SelectCharacterToggle(0);
	}
	Select_Button->OnClicked.AddDynamic(this, &UVCHUD_CharacterSelection::HandleSelectButtonClicked);
	
}

void UVCHUD_CharacterSelection::RemoveFromParent()
{
	// Unbind delegate
	if (VCPlayerController)
	{
		VCPlayerController->OnControlledCharacterChanged.RemoveAll(this);
	}
	Super::RemoveFromParent();
}

void UVCHUD_CharacterSelection::SelectCharacterToggle(int ToggleIndex)
{
	if(CharacterToggleButtons.IsValidIndex(ToggleIndex) == false){
		return;
	}
	
	for(auto& Toggle : CharacterToggleButtons)
	{
		Toggle->SetActive(false);
	}
	
	CharacterToggleButtons[ToggleIndex]->SetActive(true);
	if(ActiveIndex != ToggleIndex)
	{
		ActiveIndex = ToggleIndex;
		HandleCharacterSelect(ActiveIndex);
	}
}

void UVCHUD_CharacterSelection::HandleSelectButtonClicked()
{
	check(VCPlayerController)
	check(VCPlayerController->GetVCPlayerState())
	
	if(DisplayedCharacter)
	{
		DisplayedCharacter->Destroy();
	}
	VCPlayerController->PostLocalAkEvent(AkEvent_SelectButtonClick);
	OnCharacterSelected.Broadcast(ActiveIndex);

	// Deactivate widget when selected character is confirmed
	VCPlayerController->OnControlledCharacterChanged.AddWeakLambda(this, [=](AVCCharacter* Character)
	{
		if (VCGameState && Character && Character->IsA(VCGameState->GetPlayableCharacters()[ActiveIndex]))
		{
			SetWidgetActive(false);
		}
	});
	// RPC to select character
	VCPlayerController->ServerCreateCharacter(ActiveIndex, VCPlayerController->GetVCPlayerState()->GetTeamType());
}

void UVCHUD_CharacterSelection::HandleBackButtonClicked()
{
	
}

void UVCHUD_CharacterSelection::HandleCharacterSelect(int CharacterIndex)
{
	if(VCGameState == nullptr || VCPlayerController == nullptr){
		return;
	}
	
	if(DisplayedCharacter)
	{
		DisplayedCharacter->Destroy();
		DisplayedCharacter = nullptr;
	}

	const TSubclassOf<AVCCharacter>& SelectedCharacterClass = VCGameState->GetPlayableCharacters()[CharacterIndex];

	// Spawn character to be displayed
	DisplayedCharacter = GetWorld()->SpawnActorDeferred<AVCCharacter>(SelectedCharacterClass, DisplayedCharacterSpawnTransform);
	DisplayedCharacter->SetReplicates(false);
	DisplayedCharacter->AutoPossessAI = EAutoPossessAI::Disabled;
	DisplayedCharacter->GetCharacterMovement()->GravityScale = 0;
	DisplayedCharacter->SetActorEnableCollision(false);
	DisplayedCharacter->SetCameraView(ECameraViewTypes::Front);
	VCPlayerController->SetViewTargetWithBlend(DisplayedCharacter);
	DisplayedCharacter->SetStatusBarVisibility(false);
	UGameplayStatics::FinishSpawningActor(DisplayedCharacter, DisplayedCharacter->GetTransform());

	// Spawn particle effect
	UGameplayStatics::SpawnEmitterAtLocation(GetOuter(), SpawnParticle,
		DisplayedCharacter->GetActorLocation() + SpawnParticleTransform.GetLocation(), 
		SpawnParticleTransform.Rotator(), SpawnParticleTransform.GetScale3D(), true);
	
}