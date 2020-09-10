// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCGameTypes.h"
#include "VCUserWidget.h"
#include "VCHUD_TeamSelection.generated.h"

class UVCWidget_ToggleButton;
class UUniformGridPanel;
class UButton;
class UImage;
class AVCTeam;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class VITACORE_API UVCHUD_TeamSelection : public UVCUserWidget
{
	GENERATED_BODY()

protected:
	// Select a toggle character toggle and deselect already selected ones
	UFUNCTION() void SelectTeamToggle(int ToggleIndex);

	// Callback for when a team toggle is activated
	UFUNCTION() void HandleTeamSelect();

	// Callback for button click
	UFUNCTION() void HandleSelectButtonClicked();

	// Update player team member names
	UFUNCTION() void UpdateCurrentPlayers();
	
protected:
	// Character toggle class that will be used to create character toggles
	UPROPERTY(EditAnywhere, Category = "Config") TSubclassOf<UWidget> TeamToggleButtonClass;

	// Panel to contain character toggle buttons
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* ToggleButtonContainer_GridPanel;

	// Button widget to select team
	UPROPERTY(meta = (BindWidget)) UButton* Select_Button;

	UPROPERTY(meta = (BindWidget)) UVerticalBox* TeamPlayers_VerticalBox;
	
	UPROPERTY(meta = (BindWidget)) UVerticalBox* NoTeamPlayers_VerticalBox;


	// Background image of the menu
	UPROPERTY(meta = (BindWidget)) UImage* Background_Image;

	// References to created toggle buttones with team actors associated with them
	TArray<TPair<AVCTeam*,UVCWidget_ToggleButton*>> TeamToggleButtons;

	// Currently active
	UPROPERTY() int ActiveIndex;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
