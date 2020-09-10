// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"
#include "VCHUD_CharacterSelection.generated.h"

class AVCGameState;
class AVCCharacter;
class AVCPlayerController;
class AVCPlayerState;

class UButton;
class UUniformGridPanel;
class UVCWidget_ToggleButton;
class UAkAudioEvent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected,int)

UCLASS()
class VITACORE_API UVCHUD_CharacterSelection : public UVCUserWidget
{
	GENERATED_BODY()


public:

	FOnCharacterSelected OnCharacterSelected;
	
protected:

	// Select a toggle character toggle and deselect already selected ones
	UFUNCTION() void SelectCharacterToggle(int ToggleIndex);

	UFUNCTION() virtual void HandleSelectButtonClicked();
	UFUNCTION() virtual void HandleBackButtonClicked();
	UFUNCTION() void HandleCharacterSelect(int CharacterIndex);

protected:
	
	// Character toggle class that will be used to create character toggles
	UPROPERTY(EditAnywhere, Category = "Config") TSubclassOf<UVCWidget_ToggleButton> CharacterToggleButtonTemplate;

	// Particle to be spawned when character is created to be displayed
	UPROPERTY(EditAnywhere, Category = "Config|CharacterDisplay") UParticleSystem* SpawnParticle;

	// Custom transform for spawn particle
	UPROPERTY(EditAnywhere, Category = "Config|CharacterDisplay") FTransform SpawnParticleTransform;

	UPROPERTY(EditAnywhere, Category = "Config|CharacterDisplay") UAkAudioEvent* AkEvent_ToggleSelect;
	
	UPROPERTY(EditAnywhere, Category = "Config|CharacterDisplay") UAkAudioEvent* AkEvent_SelectButtonClick;

	// Currently active
	UPROPERTY() int ActiveIndex;

	// Button to notify option is selected
	UPROPERTY(meta = (BindWidget)) UButton* Select_Button;

	// Button to go back to the previous state of the display
	UPROPERTY(meta = (BindWidget)) UButton* Back_Button;

	// Panel to contain character toggle buttons
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* ToggleButtonContainer_GridPanel;

	
	UPROPERTY(Transient) TArray<UVCWidget_ToggleButton*> CharacterToggleButtons;

	UPROPERTY(Transient) FTransform DisplayedCharacterSpawnTransform;
	UPROPERTY(Transient) AVCCharacter* DisplayedCharacter;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;


};
