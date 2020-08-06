// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"

#include "VCHUD_Player.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UCanvasPanel;
class UWidgetSwitcher;
class AVCPlayerController;
class UGameplayAbility;
class URetainerBox;
class UOverlay;
class AVCCharacter;
class UVCHealthAttributeSet;
class UVCTraitAttributeSet;
class UVCEnergyAttributeSet;
class UAbilitySystemComponent;
class UVCWidget_AbilitySlot;


UCLASS()
class VITACORE_API UVCHUD_Player : public UVCUserWidget
{
	GENERATED_BODY()

protected:
// ================== UI ELEMENTS ==================

	// Information text about health and max health
	UPROPERTY(meta = (BindWidget)) UTextBlock* Health_TextBlock;
	
	// Progress Bar to display health ratio
	UPROPERTY(meta = (BindWidget)) UProgressBar* Health_ProgressBar;

	// Information text about energy and max energy 
	UPROPERTY(meta = (BindWidget)) UTextBlock* Energy_TextBlock;
	
	// Progress Bar to display energy ratio
	UPROPERTY(meta = (BindWidget)) UProgressBar* Energy_ProgressBar;

	// Fortitude attribute text
	UPROPERTY(meta = (BindWidget)) UTextBlock* Fortitude_TextBlock;

	// Mastery attribute text
	UPROPERTY(meta = (BindWidget)) UTextBlock* Mastery_TextBlock;

	// Movement speed attribute text
	UPROPERTY(meta = (BindWidget)) UTextBlock* Movement_TextBlock;

	// Team or character experience ratio based on level brackets
	UPROPERTY(meta = (BindWidget)) UProgressBar* XP_ProgressBar;

	// Team or character level
	UPROPERTY(meta = (BindWidget)) UTextBlock* Level_TextBlock;

	// Team logo that will be fetch from the team
	UPROPERTY(meta = (BindWidget)) UImage* TeamLogo_Image;

	// InCombat state display
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UOverlay* InCombatOverlay;

	// Damage mask to show health ratio
	UPROPERTY(meta = (BindWidget)) URetainerBox* DamageMaskRetainerBox;

	
	// Ability Slots
	UPROPERTY(meta = (BindWidget)) UVCWidget_AbilitySlot* Ability1_SlotWidget;
	UPROPERTY(meta = (BindWidget)) UVCWidget_AbilitySlot* Ability2_SlotWidget;
	UPROPERTY(meta = (BindWidget)) UVCWidget_AbilitySlot* Ability3_SlotWidget;
	UPROPERTY(meta = (BindWidget)) UVCWidget_AbilitySlot* Ability4_SlotWidget;
	UPROPERTY(meta = (BindWidget)) UVCWidget_AbilitySlot* Ability5_SlotWidget;


protected:

	// Initialize HUD by caching references and binding events
	UFUNCTION() void InitializeHUD();
	// Initialize ability slot based on ability input ID
	UFUNCTION() void InitializeAbility(int AbilityIndex, UVCWidget_AbilitySlot* AbilitySlotWidget);

	// Refresh HUD elements
	UFUNCTION() void UpdateHealth() const;
	UFUNCTION() void UpdateEnergy() const;
	UFUNCTION() void UpdateFortitude() const;
	UFUNCTION() void UpdateMastery() const;
	UFUNCTION() void UpdateMovementSpeed() const;
	UFUNCTION() void UpdateTeamLogo() const;
	UFUNCTION() void UpdateLevel(int CurrentLevel) const;
	UFUNCTION() void UpdateXP(float XPRatio) const;

	UFUNCTION() void TriggerDamageMask(float MaskRatio);

	// Blueprint events for animations
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnLevelUp"))
		void BP_OnLevelUp();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnNotifyDeath"))
		void BP_NotifyDeath();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|NotifyDuration")
		float NotifyDuration;

	// --- Damage Mask
	
	UPROPERTY(EditAnywhere, Category = "Config|DamageMask")
		float DamageMaskMinimumRadius;

	UPROPERTY(EditAnywhere, Category = "Config|DamageMask")
		float DamageMaskLerpMagnitude;
	
	FTimerHandle TimerHandle_DamageMask;
	
	// Cached references
	UPROPERTY(Transient) AVCPlayerController* MyPlayerController;	
	UPROPERTY(Transient) AVCCharacter* MyCharacter;
	UPROPERTY(Transient) UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(Transient) UVCHealthAttributeSet* HealthAttributeSet;
	UPROPERTY(Transient) UVCTraitAttributeSet* TraitAttributeSet;
	UPROPERTY(Transient) UVCEnergyAttributeSet* EnergyAttributeSet;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UCanvasPanel* CharacterInfoCanvas;


	UFUNCTION(BlueprintCallable) void NotifySpawn();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnNotifySpawn"))
		void BP_NotifySpawn();

	UFUNCTION() void NotifyDamage(float AppliedDamage);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnNotifyDamage"))
		void BP_NotifyDamage(float AppliedDamage);
};
